#include "ColorDefenseHUD.h"
#include "MainHUDWidget.h"
#include "ShopWidget.h"
#include "CreepPathWidget.h"
#include "GameOverWidget.h"

void AColorDefenseHUD::BeginPlay()
{
    Super::BeginPlay();

    if (!MainHUDClass) return;
    
    MainHUDInstance = CreateWidget<UMainHUDWidget>(GetOwningPlayerController(), MainHUDClass);
    if (!MainHUDInstance) return;

    MainHUDInstance->AddToViewport();
    MainHUDInstance->ShowGameplayUI(); // Start with the game overlay (Index 0)

    MainHUDInstance->ShopWidget->OnRequestCreepPath.AddDynamic(this, &AColorDefenseHUD::HandleShopToCreepPath);
    MainHUDInstance->ShopWidget->OnRequestQuit.AddDynamic(this, &AColorDefenseHUD::HandleQuitToGame);

    MainHUDInstance->CreepPathWidget->OnBackToShop.AddDynamic(this, &AColorDefenseHUD::HandleBackToShop);
    MainHUDInstance->CreepPathWidget->OnQuitToGame.AddDynamic(this, &AColorDefenseHUD::HandleQuitToGame);
}

// ====================== //
// ===== Toggle HUD ===== //
// ====================== //

void AColorDefenseHUD::ToggleShopHUD()
{
    // Toggle
    bIsUIVisible = !bIsUIVisible;
    
    // Show Inventory
    APlayerController* PC = GetOwningPlayerController();
    if (bIsUIVisible)
    {
        MainHUDInstance->ShowShop();
        PC->bShowMouseCursor = true;
        FInputModeGameAndUI InputMode;
        PC->SetInputMode(InputMode);
    }
    // Or Show GamePlayUI
    else
    {
        MainHUDInstance->ShowGameplayUI();
        PC->bShowMouseCursor = false;
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
    }
}

void AColorDefenseHUD::ToggleInventoryHUD()
{
    // Toggle
    bIsUIVisible = !bIsUIVisible;
    
    // Show Inventory
    APlayerController* PC = GetOwningPlayerController();
    if (bIsUIVisible)
    {
        MainHUDInstance->ShowInventory();
        PC->bShowMouseCursor = true;
        FInputModeGameAndUI InputMode;
        PC->SetInputMode(InputMode);
    }
    // Or Show GamePlayUI
    else
    {
        MainHUDInstance->ShowGameplayUI();
        PC->bShowMouseCursor = false;
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
    }
}

// Navigation Logic
void AColorDefenseHUD::HandleShopToCreepPath() { MainHUDInstance->ShowCreepPath(); }
void AColorDefenseHUD::HandleBackToShop()      { MainHUDInstance->ShowShop(); }
void AColorDefenseHUD::HandleQuitToGame()      { MainHUDInstance->ShowGameplayUI(); }
void AColorDefenseHUD::HandleGameOver()        { MainHUDInstance->ShowGameOver(); }