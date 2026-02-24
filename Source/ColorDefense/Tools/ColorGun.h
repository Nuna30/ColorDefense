// ColorGun.h
#pragma once

#include "Tool.h" 
#include "Data/Actors/CreepCore.h"
#include "GameStates/ColorDefenseGameState.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "ColorGun.generated.h"

UCLASS()
class COLORDEFENSE_API AColorGun : public ATool
{
    GENERATED_BODY()
    AColorGun();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime);

public: // --- Mesh --- //
    UPROPERTY(VisibleAnywhere, Category = "Mesh")
    USceneComponent* DefaultRoot;

    UPROPERTY(VisibleAnywhere, Category = "Mesh")
    UStaticMeshComponent* GunMeshComponent;

public: // --- Color Gun Specifics --- //
    UPROPERTY(EditAnywhere, Category = "Setting")
    float MaxRange = 5000.0f;

public: // --- Gun's Color Swap --- //
    EColor CurrentColor = EColor::Red;

    UPROPERTY(EditDefaultsOnly, Category = "Setting")
    TObjectPtr<UNiagaraSystem> SwapVFX;

    void ChangeGunColor(EColor NewColor);

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
    virtual void LeftClickReleased(); 
};