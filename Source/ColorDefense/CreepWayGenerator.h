// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "VoxelGenerator.h" // CreepWay Voxel을 생성하기 위해
#include "CreepCheckPointGenerator.h" // 코너마다 CreepCheckPoint를 설치하기 위해
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CreepWayGenerator.generated.h"

UCLASS()
class COLORDEFENSE_API UCreepWayGenerator : public UVoxelGenerator // 상속할 땐 전방 선언 불가
{
	GENERATED_BODY()

public:
	bool bTopRailIn;
	int32 MaxRailCount;
	int32 RailLength;
	TArray<TArray<FIntVector>> RailBuffers;
	TArray<FIntVector> MainBuffer;
	TArray<FIntVector> ODirectionArray;
	TArray<FIntVector> LastIndexesOfEachRail;
	TArray<UCreepCheckPointGenerator*> CreepCheckPointGenerators;
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
		int32 RailLength
	);
public:
	FIntVector GetPerpendicularDirection(const FIntVector& Direction);
	void DecideNextDirection();
	void UpdateTopRailIn();
	void UpdateLastIndexesOfEachRail();
	void SetLastIndexesOfEachRailToCreepCheckPoint();
	void RotateSlopeCreepWayBlock(const FIntVector& VoxelIndex);
public:
	void PrintRailBuffers();
	void PrintLastIndexes();
	void PrintDirections();
	void print(FString DebugMessage);
public:
	void SpawnActorWithFlushingMainBuffer();
	void FlushRailBuffersToMainBuffer();
public:
	void LoadVoxelIndexTriangleIntoRailBuffers(const FIntVector& Direction);
	void LoadVoxelIndexRectangleIntoRailBuffers(int32 BPActorPoolIndex, bool bRotate);
public:
	void InitializeCreepWay();
	void GenerateCreepWay(int32 GenerationStep);
public:
	void GoStraightAndUpOrDownAndGoStraight();
	void GoStraightAndTurnLeftOrRightAndGoStraight();
};