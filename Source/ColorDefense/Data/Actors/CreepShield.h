#pragma once

#include "Utils/GameEnums.h"
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

public: // Property
	EColor CreepShieldColor;

public: // Mesh
	UPROPERTY(EditAnywhere, Category = "Setting")
    UStaticMeshComponent* CreepShield;

public: // Utils
	// Change the CreepSheld Color dynamically
	void ChangeColor(EColor Color);
};
