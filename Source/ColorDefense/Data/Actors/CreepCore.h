#pragma once

#include "Utils/GameEnums.h"
#include "CoreMinimal.h"
#include "NiagaraFunctionLibrary.h" 
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "CreepCore.generated.h"

UCLASS()
class COLORDEFENSE_API ACreepCore : public AActor
{
	GENERATED_BODY()
	ACreepCore();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public: // --- Property --- //
	EColor CreepCoreColor;
	FVector OriginalScale; // Used in SetHighlighted

public: // --- Mesh --- //
    UPROPERTY(EditAnywhere, Category = "Setting")
    UStaticMeshComponent* CreepCore;

public: // --- Destroy --- //
	UPROPERTY(EditAnywhere, Category = "Setting")
    TObjectPtr<UNiagaraSystem> CreepCoreDestroyVFX;

    UPROPERTY(EditAnywhere, Category = "Setting")
    TObjectPtr<USoundBase> CreepCoreDestroySFX; 
	
	void HandleDestruction();

public: // --- Utils --- //
	// Change the CreepSheld Color dynamically
	void ChangeColor(EColor Color);

	// Set core highlighted when the creepCore is being dragged using ColorGun.
	void SetHighlighted(bool bHighlight);
};
