#include "ZCAMFreeDSourceFactory.h"
#include "ZCAMFreeDSource.h"
#include "Internationalization/Text.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Input/SComboBox.h"

UZCAMFreeDSourceFactory::UZCAMFreeDSourceFactory()
{

}

FText UZCAMFreeDSourceFactory::GetSourceDisplayName() const
{
    return NSLOCTEXT("ZCAMFreeD", "SourceDisplayName", "Z CAM FreeD");
}

FText UZCAMFreeDSourceFactory::GetSourceTooltip() const
{
    return NSLOCTEXT("ZCAMFreeD", "SourceTooltip", "Creates a connection to a FreeD Protocol Source");
}

TSharedPtr<ILiveLinkSource> UZCAMFreeDSourceFactory::CreateSource(const FString& ConnectionString) const
{
    TArray<FString> ConnectionParts;
    ConnectionString.ParseIntoArray(ConnectionParts, TEXT("|"));
    int32 Port = FCString::Atoi(*ConnectionParts[0]);
    FString Model = *ConnectionParts[1];
    UE_LOG(LogTemp, Warning, TEXT("CreateSource called! Port: %d, Model: %s"), Port, *Model);
    return MakeShared<FZCAMFreeDSource>(Port, Model);
}

ULiveLinkSourceFactory::EMenuType UZCAMFreeDSourceFactory::GetMenuType() const
{
    return EMenuType::SubPanel;
}

TSharedPtr<SWidget> UZCAMFreeDSourceFactory::BuildCreationPanel(FOnLiveLinkSourceCreated OnLiveLinkSourceCreated) const
{
    TSharedRef<int32> PortValue = MakeShared<int32>(5001);

    static TArray<TSharedPtr<FString>> ModuleTypeOptions;
    if (ModuleTypeOptions.Num() == 0)
    {
        ModuleTypeOptions.Add(MakeShared<FString>(TEXT("P2-R1 (18x)")));
        ModuleTypeOptions.Add(MakeShared<FString>(TEXT("P2-R1 (24x)")));
        ModuleTypeOptions.Add(MakeShared<FString>(TEXT("General")));
    }

    // 用TSharedRef<TSharedPtr<FString>>来记录当前选中项
    TSharedRef<TSharedPtr<FString>> SelectedModuleType = MakeShared<TSharedPtr<FString>>(ModuleTypeOptions[0]);

    return SNew(SVerticalBox)
    // module type
    + SVerticalBox::Slot().AutoHeight().Padding(2)
    [
        SNew(SHorizontalBox)
        + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center).Padding(2)
        [
            SNew(STextBlock).Text(FText::FromString(TEXT("Module:")))
        ]
        + SHorizontalBox::Slot().FillWidth(1.0f).VAlign(VAlign_Center).Padding(2)
        [
            SNew(SComboBox<TSharedPtr<FString>>)
            .OptionsSource(&ModuleTypeOptions)
            .OnGenerateWidget_Lambda([](TSharedPtr<FString> InOption) {
                return SNew(STextBlock).Text(FText::FromString(*InOption));
            })
            .OnSelectionChanged_Lambda([SelectedModuleType](TSharedPtr<FString> NewSelection, ESelectInfo::Type) {
                if (NewSelection.IsValid())
                {
                    *SelectedModuleType = NewSelection;
                }
            })
            .InitiallySelectedItem(*SelectedModuleType)
            [
                SNew(STextBlock)
                .Text_Lambda([SelectedModuleType]() {
                    return FText::FromString(**SelectedModuleType);
                })
            ]
        ]
    ]
    // udp port
    + SVerticalBox::Slot().AutoHeight().Padding(2)
    [
        SNew(SHorizontalBox)
        + SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center).Padding(2)
        [
            SNew(STextBlock).Text(FText::FromString(TEXT("UDP Port:")))
        ]
        + SHorizontalBox::Slot().FillWidth(1.0f).VAlign(VAlign_Center).Padding(2)
        [
            SNew(SEditableTextBox)
            .MinDesiredWidth(100)
            .Text_Lambda([PortValue]() { return FText::AsNumber(*PortValue); })
            .OnTextCommitted_Lambda([PortValue](const FText& NewText, ETextCommit::Type) {
                *PortValue = FCString::Atoi(*NewText.ToString());
            })
        ]
    ]
    // Add Button
    + SVerticalBox::Slot().AutoHeight().Padding(2)
    [
        SNew(SButton)
        .HAlign(HAlign_Center)
        [
            SNew(STextBlock)
            .Text(FText::FromString(TEXT("Add")))
            .Justification(ETextJustify::Center)
        ]
        .OnClicked_Lambda([OnLiveLinkSourceCreated, PortValue, SelectedModuleType, this]() {
            FString ConnectionString = FString::Printf(TEXT("%d|%s"), *PortValue, *(**SelectedModuleType));
            OnLiveLinkSourceCreated.ExecuteIfBound(
                MakeShared<FZCAMFreeDSource>(*PortValue, **SelectedModuleType),
                ConnectionString
            );
            return FReply::Handled();
        })
    ];
}
