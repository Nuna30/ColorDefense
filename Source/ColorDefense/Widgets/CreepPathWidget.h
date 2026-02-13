#pragma once

#include "Blueprint/UserWidget.h"
#include "CreepPathWidget.generated.h"

UCLASS()
class COLORDEFENSE_API UCreepPathWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget))
    class UButton* BuyButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* SellButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* GoBackButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* QuitButton;

    UPROPERTY(meta = (BindWidget))
    class UCanvasPanel* CreepPathCanvasPanel;

    virtual void NativeConstruct() override;

public:
    UPROPERTY(EditAnywhere, Category = "Widget")
    TSubclassOf<class UShopWidget> ShopWidgetClass;

    UPROPERTY()
    class UShopWidget* ShopWidgetInstance;
    
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