// Rifle.h
#pragma once

#include "Tool.h" 
#include "Kismet/GameplayStatics.h"
#include "Data/Actors/CreepShield.h"
#include "TimerManager.h"

#include "GameStates/ColorDefenseGameState.h"
#include "CoreMinimal.h"
#include "Rifle.generated.h"

UCLASS()
class COLORDEFENSE_API ARifle : public ATool
{
    GENERATED_BODY()
    ARifle();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime);

public: // --- Fire --- //

    // --- Properties --- //
    UPROPERTY(EditAnywhere, Category = "Setting")
    float TimeBetweenShots = 0.3f;

    UPROPERTY(EditAnywhere, Category = "Setting")
    float MaxRange = 5000.0f;

	UPROPERTY(EditAnywhere, Category = "Setting")
	float Damage = 1;

    // --- Timer --- //
    FTimerHandle AutoFireTimerHandle;

    // --- Logic --- //
    UFUNCTION()
    void Fire();

    void StartFiring();
    void StopFiring();

public: // --- Color Swap --- //
    virtual void ChangeColor(EColor NewColor) override;

public: // --- Input --- //
    virtual void LeftClick() override;
    virtual void LeftClickReleased() override;

public: // --- Shoot Animation --- //
	UFUNCTION(BlueprintImplementableEvent)
	void OnShoot(); // No code here. See Rifle blueprint.

public: // --- Utils --- //
    virtual void UnEquip() override;
};