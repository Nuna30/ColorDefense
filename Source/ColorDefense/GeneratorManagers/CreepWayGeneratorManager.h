// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Generators/CreepWayGenerator.h" // CreepWayGenerator를 관리하기 위해
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CreepWayGeneratorManager.generated.h"

UCLASS()
class COLORDEFENSE_API UCreepWayGeneratorManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	void CreateCreepWayGenerator
	(
		UWorld* InWorld, 
		UBPActorPool* InBPActorPool, 
		UVoxelGrid* InVoxelGrid, 
		TArray<UCreepCheckPointGenerator*>& InCreepCheckPointGenerators, 
		int32 InMaxRailCount, 
		int32 InRailLength,
		TArray<FIntVector>& DirectionContainer,
		float InVoxelWidth,
		float InVoxelHeight
	);
	void ClearCreepWayGenerator();
public:
	UPROPERTY()
	UCreepWayGenerator* CreepWayGenerator;
};
