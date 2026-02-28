// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Utils/GameEnums.h"
#include "Data/ActorComponents/CreepGuide.h" // 크립을 레일을 따라 이동시키기 위해
#include "Components/ChildActorComponent.h"
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

public: // --- Properties --- //
	UPROPERTY()
	EColor CreepColor;

	UPROPERTY()
	int32 RailNumber;

	UPROPERTY(EditAnywhere, Category = "Setting")
	bool DontMove = false;

public: // --- Features --- //

	// --- Destroy --- //
	void HandleDestruction();

	// --- Creep Guide --- //
	UPROPERTY()
	UCreepGuide* CreepGuide;

	UPROPERTY(EditAnywhere, Category = "Setting")
    float CreepMoveSpeed = 75;

    UPROPERTY(EditAnywhere, Category = "Setting")
    float CreepAcceptanceRadius = 40;


public: // --- Creep Core --- //
	UPROPERTY(EditAnywhere, Category = "Setting") 
	UChildActorComponent* CreepCoreComponent;

public: // --- Creep Shield --- //
	UPROPERTY(EditAnywhere, Category = "Setting")
	UChildActorComponent* CreepShieldComponent;

public: // --- Utils --- //

	// Primarily Used by Creep Pattern Generator.
	void ChangeColor(EColor Color);
};
