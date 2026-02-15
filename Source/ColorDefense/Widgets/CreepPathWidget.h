#pragma once

#include "Blueprint/UserWidget.h"
#include "CreepPathWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCreepPathBackRequest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCreepPathQuitRequest);

UCLASS()
class COLORDEFENSE_API UCreepPathWidget : public UUserWidget
{
    GENERATED_BODY()
    virtual void NativeConstruct() override;

public:
    UPROPERTY()
    FOnCreepPathBackRequest OnBackToShop;

    UPROPERTY()
    FOnCreepPathQuitRequest OnQuitToGame;

public:
    UPROPERTY(meta = (BindWidget))
    class UButton* BuyButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* SellButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* GoBackButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* QuitButton;
    
public:
    UPROPERTY()
    class UCreepWayHandler* CreepWayHandler;

public:
    UFUNCTION()
    void OnBuyClicked();
    
    UFUNCTION()
    void OnSellClicked();

    UFUNCTION()
    void OnGoBackClicked();

    UFUNCTION()
    void OnQuitClicked();
};