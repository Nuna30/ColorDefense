#pragma once

#include "Blueprint/UserWidget.h"
#include "MainHUDWidget.generated.h"

UCLASS()
class COLORDEFENSE_API UMainHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // References to your decoupled widgets
    UPROPERTY(meta = (BindWidget))
    class UShopWidget* ShopWidget;

    UPROPERTY(meta = (BindWidget))
    class UCreepPathWidget* CreepPathWidget;

    UPROPERTY(meta = (BindWidget))
    class UMoneyWidget* MoneyWidget;

    UPROPERTY(meta = (BindWidget))
    class UComboWidget* ComboWidget;

    // A Canvas or Switcher to manage visibility
    UPROPERTY(meta = (BindWidget))
    class UWidgetSwitcher* UISwitcher;

public:
    void ShowGameplayUI();
    void ShowShop();
    void ShowCreepPath();
};