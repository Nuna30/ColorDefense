// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "VoxelGenerator.h" // CreepWay Voxel을 생성하기 위해
#include "PlayerBlock.h" // Spawning Placeable Voxels
#include "CoreMinimal.h"
#include "PlayerBlockGenerator.generated.h"

UCLASS()
class COLORDEFENSE_API UPlayerBlockGenerator : public UVoxelGenerator
{
	GENERATED_BODY()
public:
	TArray<FIntVector> PlayerBlockBuffer;
public:
	UPlayerBlockGenerator();
	void Initialize
	(
		UWorld* InWorld,
		UBPActorPool* InBPActorPool,
		UVoxelGrid* InVoxelGrid,
		float InVoxelWidth,
		float InVoxelHeight
	);
	void SpawnInvisibleNeighboringPlaceables(const FIntVector& VoxelIndex);
	void Add(const FIntVector& PlayerBlockIndex);
	void GeneratePlayerBlock();
};