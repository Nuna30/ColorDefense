// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class COLORDEFENSE_API AMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
    virtual void BeginPlay() override;

    // This allows us to select our WBP_MainMenu in the Unreal Editor
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
    TSubclassOf<class UUserWidget> MainMenuWidgetClass;

    // Variable to store the created widget instance
    UPROPERTY()
    class UUserWidget* MainMenuWidgetInstance;
};
