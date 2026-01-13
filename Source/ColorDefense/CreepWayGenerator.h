// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "VoxelGenerator.h" // CreepWay Voxel을 생성하기 위해
#include "ChunkGrid.h" // CreepWay Chunk를 생성하여 ChunkGrid에 담거나 해당 청크에 CreepWay가 존재하는지 파악하기 위해
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
	UChunkGrid* ChunkGrid;
	FIntVector CurrentChunkIndex;
	TArray<FIntVector> DirectionContainer;
public:
	UCreepWayGenerator();
	void Initialize
	(
		UWorld* InWorld,
		UBPActorPool* InBPActorPool,
		UVoxelGrid* InVoxelGrid,
		UChunkGrid* InChunkGrid,
		TArray<UCreepCheckPointGenerator*>& InCreepCheckPointGenerators,
		int32 MaxRailCount,
		int32 RailLength,
		TArray<FIntVector> InDirectionContainer
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
	void SpawnAllUsingChunkGrid();
public:
	void SpawnActorWithFlushingMainBuffer();
	void FlushRailBuffersToMainBuffer();
public:
	void LoadVoxelIndexTriangleIntoRailBuffers(const FIntVector& Direction, int32 Offset);
	void LoadVoxelIndexRectangleIntoRailBuffers(int32 BPActorPoolIndex, bool bRotate);
public:
	void InitializeCreepWay();
	void GenerateCreepWay(int32 GenerationStep);
public:
	void GoStraightAndUpOrDownAndGoStraight();
	void GoStraightAndTurnLeftOrRightAndGoStraight();
public:
	void SpawnInvisibleNeighboringPlaceables(const FIntVector& VoxelIndex);
};