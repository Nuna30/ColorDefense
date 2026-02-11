// ShopWidget.cpp
#include "ShopWidget.h"
#include "Kismet/GameplayStatics.h"

void UShopWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (CreepPathButton)
    {
        // Remove first to ensure we don't bind twice
        CreepPathButton->OnClicked.RemoveDynamic(this, &UShopWidget::OnCreepPathClicked);
        CreepPathButton->OnClicked.AddDynamic(this, &UShopWidget::OnCreepPathClicked);
    }

    if (QuitButton)
    {
        QuitButton->OnClicked.RemoveDynamic(this, &UShopWidget::OnQuitClicked);
        QuitButton->OnClicked.AddDynamic(this, &UShopWidget::OnQuitClicked);
    }
}

void UShopWidget::OnCreepPathClicked()
{
    
}

void UShopWidget::OnQuitClicked()
{
    
}