// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "VoxelGenerator.h" // CreepWay Voxel을 생성하기 위해
#include "CreepCheckPointGenerator.h" // 코너마다 CreepCheckPoint를 설치하기 위해
#include "PlayerBlock.h" // Spawning Placeable Voxels
#include "CoreMinimal.h"
#include "CreepWayGenerator.generated.h"

USTRUCT()
struct FCreepWayStepHistory
{
    GENERATED_BODY()

    // The voxels spawned in this specific segment
    UPROPERTY()
    TArray<FIntVector> VoxelIndices;

    // The LastIndicesOfEachRail state BEFORE this segment was added
    UPROPERTY()
    TArray<FIntVector> PreviousRailLastIndices;

    // The CurrentDirection BEFORE this segment was added
    UPROPERTY()
    FIntVector PreviousDirection;

	// Track Checkpoints spawned in this segment.
	UPROPERTY()
    TArray<AActor*> SpawnedCheckPoints;

	// The CheckPoint counts spawned in each rail.
	UPROPERTY()
    TArray<int32> CheckPointCountsPerGenerator;
};

UCLASS()
class COLORDEFENSE_API UCreepWayGenerator : public UVoxelGenerator // 상속할 땐 전방 선언 불가
{
	GENERATED_BODY()

public:
	UPROPERTY()
	class UCreepRail* CreepRail;

	UPROPERTY()
	class UPlayerBlockGenerator* PlayerBlockGenerator;

	UPROPERTY()
	TArray<UCreepCheckPointGenerator*> CreepCheckPointGenerators;

	UPROPERTY()
	TArray<FIntVector> DirectionContainer;
public:
	bool bTopRailIn;
	FIntVector NextDirection;
	FIntVector CurrentDirection;
public: // History
	UPROPERTY()
	TArray<FCreepWayStepHistory> StepHistoryStack;

	UPROPERTY()
    TArray<FIntVector> CurrentCreepWayIndices;

	UPROPERTY()
	TArray<AActor*> CurrentCheckPoints;

	UPROPERTY()
	TArray<int32> CurrentCheckPointCounts;
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
	void GenerateStartLocation();
	void GenerateNextCreepWay();
	void GenerateCreepWay();
	bool DeleteCurrentCreepWay();
public:
	void UpdateTopRailIn();
	void SpawnCheckPointsAtLastIndices();
public:
	void SpawnActorWithFlushingMainBuffer();
	void FlushRailBuffersToMainBuffer();
public:
	void GoStraightAndUpOrDownAndGoStraight();
	void GoStraightAndTurnLeftOrRightAndGoStraight();
};