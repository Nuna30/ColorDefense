#include "GameOverHandler.h"
#include "ColorDefenseGameState.h"
#include "Widgets/ColorDefenseHUD.h"
#include "Widgets/MainHUDWidget.h"
#include "Kismet/GameplayStatics.h"

AGameOverHandler::AGameOverHandler()
{
    // No need for this actor to tick
    PrimaryActorTick.bCanEverTick = false;
}

void AGameOverHandler::BeginPlay()
{
    Super::BeginPlay();

    // Bind to the OnHPZero event if the GameState is valid
    if (AColorDefenseGameState* GS = Cast<AColorDefenseGameState>(UGameplayStatics::GetGameState(this)))
    {
        GS->OnHPZero.AddDynamic(this, &AGameOverHandler::HandleGameOver);
    }
}

void AGameOverHandler::HandleGameOver()
{
    // Get the HUD through the Player Controller
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
    {
        if (AColorDefenseHUD* MyHUD = Cast<AColorDefenseHUD>(PC->GetHUD()))
        {
            // Show GameOverWidget on the screen.
            MyHUD->HandleGameOver();

            // Set up the input mode so the player can use the "Main Menu" button.
            PC->bShowMouseCursor = true;
            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(MyHUD->MainHUDInstance->TakeWidget());
            PC->SetInputMode(InputMode);
        }
    }
}