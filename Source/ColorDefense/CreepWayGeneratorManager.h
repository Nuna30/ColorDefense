// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CreepWayGenerator.h"
#include "CreepCheckPointGenerator.h"
#include "CreepWayGeneratorManager.generated.h"

UCLASS()
class COLORDEFENSE_API UCreepWayGeneratorManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	void CreateCreepWayGenerator(UWorld* InWorld, UBPActorPool* InBPActorPool, UChunk* InChunk, TArray<UCreepCheckPointGenerator*>& InCreepCheckPointGenerators, int32 InMaxRailCount, int32 InRailLength);
	void ClearCreepWayGenerator();
public:
	UPROPERTY()
	UCreepWayGenerator* CreepWayGenerator;
};
