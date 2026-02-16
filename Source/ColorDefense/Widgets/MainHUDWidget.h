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

    // A Canvas or Switcher to manage visibility
    UPROPERTY(meta = (BindWidget))
    class UWidgetSwitcher* UISwitcher;

public:
    void ShowGameplayUI();
    void ShowShop();
    void ShowCreepPath();
};