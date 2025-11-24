// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameEnums.h"
#include "BPActorPool.h"
#include "Chunk.h"
#include "Voxel.h"
#include "VoxelGenerator.generated.h"

class UBPActorPool;
class UChunk;
struct FVoxel;

UCLASS()
class COLORDEFENSE_API UVoxelGenerator : public UObject
{
	GENERATED_BODY()
public:
	UVoxelGenerator();
	void Initialize(UWorld* InWorld, UBPActorPool* InBPActorPool, UChunk* InChunk);
public:
	void SetVoxelDataInChunk(const FIntVector& VoxelIndex, int32 BPActorPoolIndex, EVoxelProperty Property);
	void DeleteVoxelDataInChunk(const FIntVector& VoxelIndex);
	void SpawnActorFromVoxel(FVoxel& Voxel);
	void DestroyActorFromVoxel(FVoxel& Voxel);
	FTransform GetWorldTransformFromVoxelIndex(const FIntVector& VoxelIndex, float Width, float Height);
public:
	UWorld* World;
	UBPActorPool* BPActorPool;
	UChunk* Chunk;
	float VoxelWidth = 250;
	float VoxelHeight = 125;
};