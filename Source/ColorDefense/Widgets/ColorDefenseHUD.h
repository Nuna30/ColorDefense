#pragma once

#include "GameFramework/HUD.h"
#include "ColorDefenseHUD.generated.h"

UCLASS()
class COLORDEFENSE_API AColorDefenseHUD : public AHUD
{
    GENERATED_BODY()
    virtual void BeginPlay() override;

public:
    UPROPERTY(EditAnywhere, Category = "Setting")
    TSubclassOf<class UMainHUDWidget> MainHUDClass;

    UPROPERTY()
    class UMainHUDWidget* MainHUDInstance;

public:
    void ToggleMainHUD();
    bool bIsUIVisible = false;

public:
    // Internal "Routing" functions for the buttons
    UFUNCTION() void HandleShopToCreepPath();
    UFUNCTION() void HandleBackToShop();
    UFUNCTION() void HandleQuitToGame();
};