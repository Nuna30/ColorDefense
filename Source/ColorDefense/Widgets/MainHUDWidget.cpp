#include "MainHUDWidget.h"
#include "Components/WidgetSwitcher.h"
#include "ShopWidget.h"
#include "CreepPathWidget.h"

void UMainHUDWidget::ShowGameplayUI()
{
    if (UISwitcher)
    {
        // Assuming index 0 is the standard gameplay overlay 
        // (MoneyWidget, ComboWidget, etc.)
        UISwitcher->SetActiveWidgetIndex(0);
    }
}

void UMainHUDWidget::ShowShop()
{
    if (UISwitcher && ShopWidget)
    {
        UISwitcher->SetActiveWidget(ShopWidget);
    }
}

void UMainHUDWidget::ShowCreepPath()
{
    if (UISwitcher && CreepPathWidget)
    {
        UISwitcher->SetActiveWidget(CreepPathWidget);
    }
}