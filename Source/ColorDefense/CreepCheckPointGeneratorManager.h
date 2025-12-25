// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CreepCheckPointGenerator.h" // 크립 체크포인트 생성기를 관리하기 위해
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CreepCheckPointGeneratorManager.generated.h"

UCLASS()
class COLORDEFENSE_API UCreepCheckPointGeneratorManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	int32 MaxRailCount;
	TArray<UCreepCheckPointGenerator*> CreepCheckPointGenerators; // 각 레일마다 체크포인트생성기 존재
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
public:
	void CreateCreepCheckPointGenerators(UWorld* InWorld, UBPActorPool* InBPActorPool, UVoxelGrid* InVoxelGrid, int32 InMaxRailCount);
	void ClearAllCreepCheckPointGenerators();
};
