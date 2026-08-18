#pragma once
#include "LiveLinkSourceFactory.h"
#include "ZCAMFreeDSourceFactory.generated.h"

UCLASS()
class ZCAMFREED_API UZCAMFreeDSourceFactory : public ULiveLinkSourceFactory
{
    GENERATED_BODY()
public:
    UZCAMFreeDSourceFactory();
    virtual FText GetSourceDisplayName() const override;
    virtual FText GetSourceTooltip() const override;
    virtual TSharedPtr<ILiveLinkSource> CreateSource(const FString& ConnectionString) const override;
    virtual EMenuType GetMenuType() const override;
    virtual TSharedPtr<SWidget> BuildCreationPanel(FOnLiveLinkSourceCreated OnLiveLinkSourceCreated) const override;
}; 