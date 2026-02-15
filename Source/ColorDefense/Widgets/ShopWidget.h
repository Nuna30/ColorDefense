#pragma once

#include "Blueprint/UserWidget.h"
#include "ShopWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShopRequestCreepPath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShopQuitRequest);

UCLASS()
class COLORDEFENSE_API UShopWidget : public UUserWidget
{
    GENERATED_BODY()
    virtual void NativeConstruct() override;
    
public:
    UPROPERTY()
    FOnShopRequestCreepPath OnRequestCreepPath;

    UPROPERTY()
    FOnShopQuitRequest OnRequestQuit;

public:
    UPROPERTY(meta = (BindWidget))
    class UButton* CreepPathButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* QuitButton;

public:
    UFUNCTION()
    void OnCreepPathClicked();

    UFUNCTION()
    void OnQuitClicked();
};