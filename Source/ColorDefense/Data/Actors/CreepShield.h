#pragma once

#include "Utils/GameEnums.h"
#include "NiagaraFunctionLibrary.h" 
#include "NiagaraComponent.h" 


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CreepShield.generated.h"

UCLASS()
class COLORDEFENSE_API ACreepShield : public AActor
{
	GENERATED_BODY()
	ACreepShield();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public: // --- Property --- //
	EColor CreepShieldColor;

	UPROPERTY(EditAnywhere, Category = "Setting")
	int32 CreepShieldHP = 10;

public: // --- Mesh --- //
	UPROPERTY(EditAnywhere, Category = "Setting")
    UStaticMeshComponent* CreepShield;

public: // --- Features --- //

	// --- Hit --- //
	UPROPERTY(EditAnywhere, Category = "Setting")
    TObjectPtr<UNiagaraSystem> CreepShieldHitVFX;

    UPROPERTY(EditAnywhere, Category = "Setting")
    TObjectPtr<USoundBase> CreepShieldHitSFX; 

	UFUNCTION()
	void OnHit(int32 Damage);

	// --- Destroy --- //
	UPROPERTY(EditAnywhere, Category = "Setting")
    TObjectPtr<UNiagaraSystem> CreepShieldDestroyVFX;

    UPROPERTY(EditAnywhere, Category = "Setting")
    TObjectPtr<USoundBase> CreepShieldDestroySFX; 
	
	void HandleDestruction();

public: // --- Utils --- //
	// Change the CreepSheld Color dynamically
	void ChangeColor(EColor Color);
};
