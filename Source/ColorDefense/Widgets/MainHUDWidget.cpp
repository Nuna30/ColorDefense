#include "MainHUDWidget.h"
#include "Components/WidgetSwitcher.h"
#include "ShopWidget.h"
#include "CreepPathWidget.h"
#include "GameOverWidget.h"

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

void UMainHUDWidget::ShowGameOver()
{
    if (UISwitcher && GameOverWidget)
    {
        UISwitcher->SetActiveWidget(GameOverWidget);
    }
}