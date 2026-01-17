// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "VoxelGenerator.h" // CreepWay Voxel을 생성하기 위해
#include "CreepCheckPointGenerator.h" // 코너마다 CreepCheckPoint를 설치하기 위해
#include "PlayerBlock.h" // Spawning Placeable Voxels
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CreepWayGenerator.generated.h"

UCLASS()
class COLORDEFENSE_API UCreepWayGenerator : public UVoxelGenerator // 상속할 땐 전방 선언 불가
{
	GENERATED_BODY()

public:
	class UCreepRail* CreepRail;
	TArray<UCreepCheckPointGenerator*> CreepCheckPointGenerators;
	TArray<FIntVector> DirectionContainer;
public:
	bool bTopRailIn;
	FIntVector NextDirection;
	FIntVector CurrentDirection;
public:
	UCreepWayGenerator();
	void Initialize
	(
		UWorld* InWorld,
		UBPActorPool* InBPActorPool,
		UVoxelGrid* InVoxelGrid,
		TArray<UCreepCheckPointGenerator*>& InCreepCheckPointGenerators,
		int32 MaxRailCount,
		int32 RailLength,
		TArray<FIntVector> InDirectionContainer,
		float InVoxelWidth,
		float InVoxelHeight
	);
public:
	void GenerateCreepWay(int32 GenerationStep);
public:
	void UpdateTopRailIn();
	void SetLastIndexesOfEachRailToCreepCheckPoint();
public:
	void SpawnActorWithFlushingMainBuffer();
	void FlushRailBuffersToMainBuffer();
public:
	void GoStraightAndUpOrDownAndGoStraight();
	void GoStraightAndTurnLeftOrRightAndGoStraight();
public:
	void SpawnInvisibleNeighboringPlaceables(const FIntVector& VoxelIndex);
};