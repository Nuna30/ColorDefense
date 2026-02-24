// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Data/ActorComponents/CreepGuide.h" // 크립을 레일을 따라 이동시키기 위해
#include "Components/ChildActorComponent.h"
#include "AIController.h" // 크립의 제어권을 AI에게 넘기기 위해
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Creep.generated.h"

UCLASS()
class COLORDEFENSE_API ACreep : public APawn
{
	GENERATED_BODY()
	ACreep();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public: // --- Creep Property --- //
	UPROPERTY()
	EColor CreepColor;

	UPROPERTY()
	int32 RailNumber;

	UPROPERTY(EditAnywhere, Category = "Setting")
	bool DontMove = false;

public: // --- Creep Core --- //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting") 
	UChildActorComponent* CreepCoreComponent;

public: // --- AI --- //
	UPROPERTY()
	AAIController* AIController;

	UPROPERTY()
	UCreepGuide* CreepGuide;

	// After Possession, CreepGuide guides this creep.
	virtual void PossessedBy(AController* NewController) override;

public: // --- Utils --- //
	// Other classes can destroy this creep via this function.
	void HandleDestruction();

	// Primarily Used by Creep Pattern Generator.
	void ChangeColor(EColor Color);
};
