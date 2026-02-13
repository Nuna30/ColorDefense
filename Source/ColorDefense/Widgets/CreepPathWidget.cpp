// CreepPathWidget.cpp
#include "CreepPathWidget.h"
#include "CreepWayHandler.h"
#include "ColorDefenseGameState.h"
#include "ShopWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"

void UCreepPathWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Initialize the CreepWayHandler
    if (!CreepWayHandler)
    {
        CreepWayHandler = NewObject<UCreepWayHandler>(this);
        CreepWayHandler->Initialize();
    }

    if (BuyButton)
    {
        // Remove first to ensure we don't bind twice
        BuyButton->OnClicked.RemoveDynamic(this, &UCreepPathWidget::OnBuyClicked);
        BuyButton->OnClicked.AddDynamic(this, &UCreepPathWidget::OnBuyClicked);
    }

    if (SellButton)
    {
        SellButton->OnClicked.RemoveDynamic(this, &UCreepPathWidget::OnSellClicked);
        SellButton->OnClicked.AddDynamic(this, &UCreepPathWidget::OnSellClicked);
    }

    if (GoBackButton)
    {
        GoBackButton->OnClicked.RemoveDynamic(this, &UCreepPathWidget::OnGoBackClicked);
        GoBackButton->OnClicked.AddDynamic(this, &UCreepPathWidget::OnGoBackClicked);
    }

    if (QuitButton)
    {
        QuitButton->OnClicked.RemoveDynamic(this, &UCreepPathWidget::OnQuitClicked);
        QuitButton->OnClicked.AddDynamic(this, &UCreepPathWidget::OnQuitClicked);
    }
}

void UCreepPathWidget::OnBuyClicked()
{
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Buy a Creep Path"));

    // Try to buy a chunk of CreepPath
    AColorDefenseGameState* ColorDefenseGameState = GetWorld()->GetGameState<AColorDefenseGameState>();
    if (ColorDefenseGameState && CreepWayHandler)
    {
        // Spend 100 gold
        if (ColorDefenseGameState->TrySpendMoney(100))
        {
            CreepWayHandler->BuildCreepWay();
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Not enough money!"));
        }
    }    
}

void UCreepPathWidget::OnSellClicked()
{
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Sell a Creep Path"));

    // Try to sell a chunk of CreepPath
    auto* ColorDefenseGameState = GetWorld()->GetGameState<AColorDefenseGameState>();
    if (ColorDefenseGameState && CreepWayHandler)
    {
        // Destruct and gain 25 gold
        CreepWayHandler->DestructCreepWay();
        ColorDefenseGameState->AddMoney(25);
    }
}

void UCreepPathWidget::OnGoBackClicked()
{
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Go back to the shop"));
    // Hide this widget
    this->SetVisibility(ESlateVisibility::Collapsed);

    // Show the Shop widget
    if (ShopWidgetInstance && ShopWidgetInstance->ShopCanvasPanel)
    {
        ShopWidgetInstance->ShopCanvasPanel->SetVisibility(ESlateVisibility::Visible);
        
        // Return focus to the Shop
        if (APlayerController* PC = GetOwningPlayer())
        {
            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(ShopWidgetInstance->TakeWidget());
            PC->SetInputMode(InputMode);
        }
    }
}

void UCreepPathWidget::OnQuitClicked()
{
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Quit"));

    // Hide the widget and return to Game Mode
    this->SetVisibility(ESlateVisibility::Collapsed);

    if (APlayerController* PC = GetOwningPlayer())
    {
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
        PC->bShowMouseCursor = false;
    }    
}