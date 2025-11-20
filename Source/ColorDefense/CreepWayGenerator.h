// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "VoxelGenerator.h"
#include "CreepWayGenerator.generated.h"

class UBPActorPool;

UCLASS()
class COLORDEFENSE_API UCreepWayGenerator : public UVoxelGenerator // 상속할 땐 전방 선언 불가
{
	GENERATED_BODY()
public:
	UCreepWayGenerator();
	void Init
	(
		UWorld* InWorld,
		UBPActorPool* InBPActorPool,
		UChunk* InChunk,
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
	void GenerateCreepWay();
public:
	void GoStraightAndUpOrDownAndGoStraight();
	void GoStraightAndTurnLeftOrRightAndGoStraight();
public:
	bool bTopRailIn;
	int32 MaxRailCount;
	int32 RailLength;
	TArray<TArray<FIntVector>> RailBuffers;
	TArray<FIntVector> MainBuffer;
	TArray<FIntVector> ODirectionArray;
	TArray<FIntVector> LastIndexesOfEachRail;
	FIntVector NextDirection;
	FIntVector CurrentDirection;
};