#include "ColorDefenseHUD.h"
#include "MainHUDWidget.h"
#include "ShopWidget.h"
#include "CreepPathWidget.h"
#include "MoneyWidget.h"
#include "ComboWidget.h"

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

void AColorDefenseHUD::ToggleMainHUD()
{
    if (!MainHUDInstance) return;
    APlayerController* PC = GetOwningPlayerController();

    bIsUIVisible = !bIsUIVisible;

    if (bIsUIVisible)
    {
        MainHUDInstance->ShowShop(); // Show the shop layer
        PC->bShowMouseCursor = true;
        FInputModeGameAndUI InputMode;
        PC->SetInputMode(InputMode);
    }
    else
    {
        MainHUDInstance->ShowGameplayUI(); // Go back to money/combo layer
        PC->bShowMouseCursor = false;
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
    }
}

void AColorDefenseHUD::UpdateMoney(int32 NewAmount)
{
    if (MainHUDInstance && MainHUDInstance->MoneyWidget)
        MainHUDInstance->MoneyWidget->UpdateMoneyText(NewAmount);
}

void AColorDefenseHUD::UpdateCombo(int32 NewCombo)
{
    if (MainHUDInstance && MainHUDInstance->ComboWidget)
        MainHUDInstance->ComboWidget->UpdateComboText(NewCombo);
}

// Navigation Logic
void AColorDefenseHUD::HandleShopToCreepPath() { MainHUDInstance->ShowCreepPath(); }
void AColorDefenseHUD::HandleBackToShop()      { MainHUDInstance->ShowShop(); }
void AColorDefenseHUD::HandleQuitToGame()      { MainHUDInstance->ShowGameplayUI(); }