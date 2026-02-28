// CoreRemover.h
#pragma once

#include "Tool.h" 
#include "Data/Actors/CreepCore.h"
#include "Data/Pawns/Creep.h"
#include "GameStates/ColorDefenseGameState.h"

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "CoreRemover.generated.h"

UCLASS()
class COLORDEFENSE_API ACoreRemover : public ATool
{
    GENERATED_BODY()
    ACoreRemover();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime);

public: // --- Properties --- //
    UPROPERTY(EditAnywhere, Category = "Setting")
    float MaxRange = 5000.0f;

public: // --- Color Swap --- //
    virtual void ChangeColor(EColor NewColor) override;

public: // --- Combo --- //
    int32 ComboCount = 0;

    // Control how much the pitch increases per combo
    UPROPERTY(EditAnywhere, Category = "Setting")
    float PitchMultiplier = 0.1f;

    UPROPERTY(EditAnywhere, Category = "Setting")
    TObjectPtr<USoundBase> ComboSFX; 
    
    void PlayComboSound();
    
public: // --- Chain Kill --- //
    UPROPERTY()
    TArray<ACreepCore*> ConnectedCreepCores;

    // Flag to track the "Hold" state
    bool bIsConnecting = false;

    // Helper to check if a creep is already in the chain
    bool IsAlreadyConnected(ACreepCore* Creep);

    // Logic to add creeps to the chain during Tick
    void UpdateChain();

public: // --- Click --- //
    virtual void LeftClick() override;
    virtual void LeftClickReleased() override;

public: // --- Shoot Animation --- //
	UFUNCTION(BlueprintImplementableEvent)
	void OnShoot(); // No code here. See CoreRemover blueprint.
};