// CreepPathWidget.cpp
#include "CreepPathWidget.h"
#include "CreepWayHandler.h"
#include "ColorDefenseGameState.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UCreepPathWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (!CreepWayHandler)
    {
        CreepWayHandler = NewObject<UCreepWayHandler>(this);
        CreepWayHandler->Initialize();
    }

    if (BuyButton) BuyButton->OnClicked.AddUniqueDynamic(this, &UCreepPathWidget::OnBuyClicked);
    if (SellButton) SellButton->OnClicked.AddUniqueDynamic(this, &UCreepPathWidget::OnSellClicked);
    if (GoBackButton) GoBackButton->OnClicked.AddUniqueDynamic(this, &UCreepPathWidget::OnGoBackClicked);
    if (QuitButton) QuitButton->OnClicked.AddUniqueDynamic(this, &UCreepPathWidget::OnQuitClicked);
}

void UCreepPathWidget::OnBuyClicked()
{
    AColorDefenseGameState* GS = GetWorld()->GetGameState<AColorDefenseGameState>();
    if (GS && CreepWayHandler && GS->TrySpendMoney(100))
    {
        CreepWayHandler->BuildCreepWay();
    }
}

void UCreepPathWidget::OnSellClicked()
{
    AColorDefenseGameState* GS = GetWorld()->GetGameState<AColorDefenseGameState>();
    if (GS && CreepWayHandler)
    {
        CreepWayHandler->DestructCreepWay();
        GS->AddMoney(25);
    }
}

void UCreepPathWidget::OnGoBackClicked()
{
    if (OnBackToShop.IsBound())
    {
        OnBackToShop.Broadcast();
    }
}

void UCreepPathWidget::OnQuitClicked()
{
    if (OnQuitToGame.IsBound())
    {
        OnQuitToGame.Broadcast();
    }
}