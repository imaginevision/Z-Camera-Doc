#pragma once

#include "CoreMinimal.h"
#include "ILiveLinkSource.h"
#include "LiveLinkSourceSettings.h"
#include "Sockets.h"
#include "HAL/RunnableThread.h"
#include "Common/UdpSocketReceiver.h"
#include "ZCAMFreeDSource.generated.h"

UCLASS()
class UZCAMFreeDConnectionSettings : public ULiveLinkSourceSettings
{
    GENERATED_BODY()

public:
    UZCAMFreeDConnectionSettings();

    UPROPERTY(EditAnywhere, Category = "FreeD Settings")
    int32 Port;
};

class FZCAMFreeDSource : public ILiveLinkSource
{
public:
    enum class EModuleType
    {
        P2_R1_18X,
        P2_R1_24X,
        General
    };

    FZCAMFreeDSource(int32 InPort, const FString& modle);
    virtual ~FZCAMFreeDSource();

    // Begin ILiveLinkSource Interface
    virtual void ReceiveClient(ILiveLinkClient* InClient, FGuid InSourceGuid) override;
    virtual void InitializeSettings(ULiveLinkSourceSettings* Settings) override;
    virtual void Update() override;
    virtual bool IsSourceStillValid() const override;
    virtual bool RequestSourceShutdown() override;
    virtual FText GetSourceType() const override;
    virtual FText GetSourceMachineName() const override;
    virtual FText GetSourceStatus() const override;
    virtual TSubclassOf<ULiveLinkSourceSettings> GetSettingsClass() const override;
    virtual void OnSettingsChanged(ULiveLinkSourceSettings* Settings, const FPropertyChangedEvent& PropertyChangedEvent) override;
    // End ILiveLinkSource Interface

    TSharedPtr<FZCAMFreeDSource> CreateSource(int32 Port, const FString& modle);

private:
    struct FreeDData
    {
        uint8 CameraID;
        FVector Location;
        FRotator Rotation;
        float FocalLength;
        float FocusDistance;
        float Aperture;
    };

    ILiveLinkClient* Client;
    FGuid SourceGuid;
    int32 Port;
    FString Model;
    EModuleType ModuleType;
    bool bIsValid;

    FSocket* UdpSocket = nullptr;
    TSharedPtr<FUdpSocketReceiver> UdpReceiver;
    FCriticalSection DataMutex;
    TArray<uint8> LastRawData;

    // LiveLink data members
    FreeDData LastFreeDData;
    bool bHasNewData = false;
    FName SubjectName;
    uint64 FrameCounter = 0;
    bool bSubjectCreated = false;

    void StartUDP();
    void StopUDP();
    void OnUDPDataReceived(const FArrayReaderPtr& Data, const FIPv4Endpoint& Endpoint);
    bool ParseFreeD(const TArray<uint8>& Data, FreeDData& OutFreeDData);
    
    // LiveLink data push methods
    void PushDataToLiveLink();
    void CreateLiveLinkSubject();

    // FreeD
    float Decode_Signed_8_15(const uint8* InBytes);
	float Decode_Signed_17_6(const uint8* InBytes);
	uint32 Decode_Unsigned_24(const uint8* InBytes);
	uint16 Decode_Unsigned_16(const uint8* InBytes);
	uint8 CalculateChecksum(const uint8* InBytes, uint32 Size);
}; 