// Rifle.h
#pragma once

#include "Tool.h" 
#include "Kismet/GameplayStatics.h"
#include "Data/Actors/CreepShield.h"

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

public: // --- Properties --- //
    UPROPERTY(EditAnywhere, Category = "Setting")
    float MaxRange = 5000.0f;

	UPROPERTY(EditAnywhere, Category = "Setting")
	float Damage = 1;

public: // --- Color Swap --- //
    virtual void ChangeColor(EColor NewColor) override;

public: // --- Click --- //
    virtual void LeftClick() override;

public: // --- Shoot Animation --- //
	UFUNCTION(BlueprintImplementableEvent)
	void OnShoot(); // No code here. See Rifle blueprint.
};