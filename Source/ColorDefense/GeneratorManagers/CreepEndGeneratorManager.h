// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Generators/CreepEndGenerator.h" // 크립 체크포인트 생성기를 관리하기 위해
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CreepEndGeneratorManager.generated.h"

UCLASS()
class COLORDEFENSE_API UCreepEndGeneratorManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
	UCreepEndGenerator* CreepEndGenerator; // 각 레일마다 체크포인트생성기 존재
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
public:
	void CreateCreepEndGenerator(UWorld* InWorld, UBPActorPool* InBPActorPool, UVoxelGrid* InVoxelGrid, int32 InMaxRailCount, float InVoxelWidth, float InVoxelHeight);
};
