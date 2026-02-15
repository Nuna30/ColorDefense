// ShopWidget.cpp
#include "ShopWidget.h"
#include "Components/Button.h"

void UShopWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (CreepPathButton) CreepPathButton->OnClicked.AddUniqueDynamic(this, &UShopWidget::OnCreepPathClicked);
    if (QuitButton) QuitButton->OnClicked.AddUniqueDynamic(this, &UShopWidget::OnQuitClicked);
}

void UShopWidget::OnCreepPathClicked()
{
    if (OnRequestCreepPath.IsBound())
    {
        OnRequestCreepPath.Broadcast();
    }
}

void UShopWidget::OnQuitClicked()
{
    if (OnRequestQuit.IsBound())
    {
        OnRequestQuit.Broadcast();
    }
}