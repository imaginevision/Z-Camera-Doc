#include "ZCAMFreeDSource.h"
#include "LiveLinkTypes.h"
#include "Roles/LiveLinkCameraRole.h"
#include "Roles/LiveLinkCameraTypes.h"

#include "ILiveLinkClient.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "SocketTypes.h"
#include "Common/UdpSocketBuilder.h"
#include "Common/UdpSocketReceiver.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"


#include "FreeDPacketInfo.h"


// These consts must be defined here in the CPP for non-MS compiler issues
const uint8 FreeDPacketDefinition::PacketTypeD1 = 0xD1;
const uint8 FreeDPacketDefinition::PacketSizeD1 = 0x1D;
const uint8 FreeDPacketDefinition::PacketType = 0x00;
const uint8 FreeDPacketDefinition::CameraID = 0x01;
const uint8 FreeDPacketDefinition::Yaw = 0x02;
const uint8 FreeDPacketDefinition::Pitch = 0x05;
const uint8 FreeDPacketDefinition::Roll = 0x08;
const uint8 FreeDPacketDefinition::X = 0x0B;
const uint8 FreeDPacketDefinition::Y = 0x0E;
const uint8 FreeDPacketDefinition::Z = 0x11;
const uint8 FreeDPacketDefinition::FocalLength = 0x14;
const uint8 FreeDPacketDefinition::FocusDistance = 0x17;
const uint8 FreeDPacketDefinition::UserDefined = 0x1A;
const uint8 FreeDPacketDefinition::Checksum = 0x1C;

UZCAMFreeDConnectionSettings::UZCAMFreeDConnectionSettings()
{
    Port = 5001; // Default FreeD port
}

FZCAMFreeDSource::FZCAMFreeDSource(int32 InPort, const FString& InModel)
    : Client(nullptr)
    , Port(InPort)
    , Model(InModel)
    , bIsValid(true)
{
    // UE_LOG(LogTemp, Warning, TEXT("FZCAMFreeDSource constructor called! Port: %d"), Port);

    if (Model == TEXT("P2-R1 (18x)")) {
        ModuleType = EModuleType::P2_R1_18X;
    } else if (Model == TEXT("P2-R1 (24x)")) {
        ModuleType = EModuleType::P2_R1_24X;
    } else {
        ModuleType = EModuleType::General;
    }
}

FZCAMFreeDSource::~FZCAMFreeDSource()
{
    bIsValid = false;
    StopUDP();
}

void FZCAMFreeDSource::ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid)
{
    Client = InClient;
    SourceGuid = InSourceGuid;
    bSubjectCreated = false;
    FrameCounter = 0;
    StartUDP();
}

void FZCAMFreeDSource::InitializeSettings(ULiveLinkSourceSettings* Settings)
{
    if (UZCAMFreeDConnectionSettings* ConnectionSettings = Cast<UZCAMFreeDConnectionSettings>(Settings))
    {
        Port = ConnectionSettings->Port;
        // We will handle port changes here in the next step
    }
}

bool FZCAMFreeDSource::RequestSourceShutdown()
{
    bIsValid = false;
    StopUDP();
    return true;
}

bool FZCAMFreeDSource::IsSourceStillValid() const
{
    return bIsValid;
}

FText FZCAMFreeDSource::GetSourceType() const
{
    return FText::FromString(TEXT("Z CAM FreeD"));
}

FText FZCAMFreeDSource::GetSourceMachineName() const
{
    return FText::FromString(FString::Printf(TEXT("On Port: %d"), Port));
    // return FText::FromString(FPlatformProcess::ComputerName());
}

FText FZCAMFreeDSource::GetSourceStatus() const
{
    return FText::FromString(TEXT("Receiving"));
}

TSharedPtr<FZCAMFreeDSource> FZCAMFreeDSource::CreateSource(int32 InPort, const FString& InModel)
{
    return MakeShared<FZCAMFreeDSource>(InPort, InModel);
}

void FZCAMFreeDSource::Update()
{
    if (bHasNewData && Client)
    {
        if (!bSubjectCreated && !SubjectName.IsNone())
        {
            CreateLiveLinkSubject();
            bSubjectCreated = true;
        }
        PushDataToLiveLink();
        bHasNewData = false;
    }
}

void FZCAMFreeDSource::CreateLiveLinkSubject()
{
    if (!Client) return;

    // Create LiveLink subject
    FLiveLinkSubjectKey SubjectKey(SourceGuid, SubjectName);
    
    // Create static data (subject properties)
    FLiveLinkStaticDataStruct StaticData(FLiveLinkCameraStaticData::StaticStruct());
    FLiveLinkCameraStaticData* CameraData = StaticData.Cast<FLiveLinkCameraStaticData>();
    if (CameraData)
    {
        // transform
        CameraData->bIsLocationSupported = false;
        CameraData->bIsRotationSupported = true;
        CameraData->bIsScaleSupported = false;
        // camera
        if (ModuleType == EModuleType::P2_R1_18X) {
            CameraData->FilmBackWidth = 9.19f;
            CameraData->FilmBackHeight = 5.17f;
        } else if (ModuleType == EModuleType::P2_R1_24X) {
            CameraData->FilmBackWidth = 7.74f;
            CameraData->FilmBackHeight = 4.36f;
        }
        CameraData->bIsApertureSupported = true;
        CameraData->bIsFocusDistanceSupported = true;
        CameraData->bIsFocalLengthSupported = true;
    }

    // Push static data
    Client->PushSubjectStaticData_AnyThread(SubjectKey, ULiveLinkCameraRole::StaticClass(), MoveTemp(StaticData));
    
    // UE_LOG(LogTemp, Warning, TEXT("Created LiveLink subject: %s"), *SubjectName.ToString());
}

void FZCAMFreeDSource::PushDataToLiveLink()
{
    if (!Client) return;

    FLiveLinkSubjectKey SubjectKey(SourceGuid, SubjectName);
    
    // Create frame data
    FLiveLinkFrameDataStruct FrameData(FLiveLinkCameraFrameData::StaticStruct());
    FLiveLinkCameraFrameData* CameraFrameData = FrameData.Cast<FLiveLinkCameraFrameData>();
    if (CameraFrameData)
    {    
        CameraFrameData->Transform = FTransform(LastFreeDData.Rotation, LastFreeDData.Location);
        CameraFrameData->FocalLength = LastFreeDData.FocalLength; // mm
        CameraFrameData->FocusDistance = LastFreeDData.FocusDistance; // cm
        CameraFrameData->Aperture = LastFreeDData.Aperture; // f-stop
    }

    // Push frame data
    Client->PushSubjectFrameData_AnyThread(SubjectKey, MoveTemp(FrameData));

    FrameCounter++;
}

void FZCAMFreeDSource::OnUDPDataReceived(const FArrayReaderPtr& Data, const FIPv4Endpoint& Endpoint)
{
    FScopeLock Lock(&DataMutex);
    LastRawData.SetNumUninitialized(Data->Num());
    FMemory::Memcpy(LastRawData.GetData(), Data->GetData(), Data->Num());
    
    FreeDData FreeDData;
    
    if (ParseFreeD(LastRawData, FreeDData))
    {
        if (!bSubjectCreated || FreeDData.CameraID != LastFreeDData.CameraID)
        {
            SubjectName = FName(*FString::Printf(TEXT("%s %d"), *Model, FreeDData.CameraID));
            // CreateLiveLinkSubject();
            // bSubjectCreated = true;
            bSubjectCreated = false;
        }
        
        LastFreeDData = FreeDData;
        bHasNewData = true;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("FreeD Parse Failed. Data size: %d"), Data->Num());
    }
}

bool FZCAMFreeDSource::ParseFreeD(const TArray<uint8>& Data, FreeDData& OutFreeDData)
{
    if (Data.Num() < FreeDPacketDefinition::PacketSizeD1) return false;

    if (Data[FreeDPacketDefinition::PacketType] != FreeDPacketDefinition::PacketTypeD1) return false;

    uint8 CameraID = Data[FreeDPacketDefinition::CameraID];

    FRotator Orientation;
    Orientation.Yaw = Decode_Signed_8_15(&Data[FreeDPacketDefinition::Yaw]);
    Orientation.Pitch = Decode_Signed_8_15(&Data[FreeDPacketDefinition::Pitch]);
    Orientation.Roll = Decode_Signed_8_15(&Data[FreeDPacketDefinition::Roll]);

    // FreeD has the X and Y axes flipped from Unreal
    FVector Position;
    Position.X = Decode_Signed_17_6(&Data[FreeDPacketDefinition::Y]);
    Position.Y = Decode_Signed_17_6(&Data[FreeDPacketDefinition::X]);
    Position.Z = Decode_Signed_17_6(&Data[FreeDPacketDefinition::Z]);

    int32 FocalLengthInt = Decode_Unsigned_24(&Data[FreeDPacketDefinition::FocalLength]);
    int32 FocusDistanceInt = Decode_Unsigned_24(&Data[FreeDPacketDefinition::FocusDistance]);
    int32 UserDefinedDataInt = Decode_Unsigned_16(&Data[FreeDPacketDefinition::UserDefined]);

    float FocalLength = FocalLengthInt / 100.0f; // 9.56mm
    float FocusDistance = FocusDistanceInt / 10.0f; // 0.1 cm
    float Aperture = (UserDefinedDataInt & 0xfff) / 100.0f; // F 1.40 f-stop 

    uint8 Checksum = CalculateChecksum(&Data[FreeDPacketDefinition::PacketType], Data.Num() - 1);
    if (Checksum != Data[FreeDPacketDefinition::Checksum])
    {
        UE_LOG(LogTemp, Error, TEXT("FreeD Checksum Failed. Data size: %d"), Data.Num());
        return false;
    }

    OutFreeDData.CameraID = CameraID;
    OutFreeDData.Location = Position;
    OutFreeDData.Rotation = Orientation;
    OutFreeDData.FocalLength = FocalLength;
    OutFreeDData.FocusDistance = FocusDistance;
    OutFreeDData.Aperture = Aperture;

    return true;
}

float FZCAMFreeDSource::Decode_Signed_8_15(const uint8* InBytes)
{
	int32 ret = (*InBytes << 16) | (*(InBytes + 1) << 8) | *(InBytes + 2);
	if (*InBytes & 0x80)
	{
		ret -= 0x00ffffff;
	}
	return (float)ret / 32768.0f;
}

float FZCAMFreeDSource::Decode_Signed_17_6(const uint8* InBytes)
{
	int32 ret = (*InBytes << 16) | (*(InBytes + 1) << 8) | *(InBytes + 2);
	if (*InBytes & 0x80)
	{
		ret -= 0x00ffffff;
	}
	return (float)ret / 640.0f;
}

uint32 FZCAMFreeDSource::Decode_Unsigned_24(const uint8* InBytes)
{
	uint32 ret = (*InBytes << 16) | (*(InBytes + 1) << 8) | *(InBytes + 2);
	return ret;
}

uint16 FZCAMFreeDSource::Decode_Unsigned_16(const uint8* InBytes)
{
	uint16 ret = (*InBytes << 8) | *(InBytes + 1);
	return ret;
}

uint8 FZCAMFreeDSource::CalculateChecksum(const uint8* InBytes, uint32 Size)
{
	uint8 sum = 0x40;
	for (uint32 i = 0; i < Size; i++)
	{
		sum -= *(InBytes + i);
	}
	return sum;
}

TSubclassOf<ULiveLinkSourceSettings> FZCAMFreeDSource::GetSettingsClass() const { return ULiveLinkSourceSettings::StaticClass(); }
void FZCAMFreeDSource::OnSettingsChanged(ULiveLinkSourceSettings* Settings, const FPropertyChangedEvent& PropertyChangedEvent) {}

void FZCAMFreeDSource::StartUDP()
{
    StopUDP();
    
    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    if (!SocketSubsystem)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get socket subsystem"));
        return;
    }
    
    FIPv4Endpoint Endpoint(FIPv4Address::Any, Port);
    UdpSocket = FUdpSocketBuilder(TEXT("FreeD_UDP_Socket"))
        .AsNonBlocking()
        .AsReusable()
        .BoundToEndpoint(Endpoint)
        .WithReceiveBufferSize(1024);
        
    if (UdpSocket)
    {
        UdpReceiver = MakeShared<FUdpSocketReceiver>(UdpSocket, FTimespan::FromMilliseconds(1), TEXT("FreeD_UDP_Receiver"));
        UdpReceiver->OnDataReceived().BindRaw(this, &FZCAMFreeDSource::OnUDPDataReceived);
        UdpReceiver->Start();
        // UE_LOG(LogTemp, Warning, TEXT("FreeD UDP listening on port %d"), Port);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create UDP socket on port %d"), Port);
        if (UdpSocket)
        {
            SocketSubsystem->DestroySocket(UdpSocket);
            UdpSocket = nullptr;
        }
    }
}

void FZCAMFreeDSource::StopUDP()
{
    if (UdpReceiver.IsValid())
    {
        UdpReceiver->Stop();
        UdpReceiver.Reset();
    }
    
    if (UdpSocket)
    {
        UdpSocket->Close();
        ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
        if (SocketSubsystem)
        {
            SocketSubsystem->DestroySocket(UdpSocket);
        }
        UdpSocket = nullptr;
    }
}