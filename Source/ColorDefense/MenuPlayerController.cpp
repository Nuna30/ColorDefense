// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuPlayerController.h"

void AMenuPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // 1. Create the widget
    MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);

    // 2. Add it to the screen
    MainMenuWidgetInstance->AddToViewport();

    // ... Your existing Input Mode logic from before ...
    FInputModeUIOnly InputModeData;
    SetInputMode(InputModeData);
    bShowMouseCursor = true;
}