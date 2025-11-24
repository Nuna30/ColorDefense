// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "VoxelGenerator.h"
#include "CreepCheckPointGenerator.generated.h"

class UVoxelGenerator;

UCLASS()
class COLORDEFENSE_API UCreepCheckPointGenerator : public UVoxelGenerator
{
	GENERATED_BODY()
public:
	UCreepCheckPointGenerator();
	void Initialize
	(
		UWorld* InWorld,
		UBPActorPool* InBPActorPool,
		UChunk* InChunk
	);
public:
	TArray<FVector> CreepCheckPointLocations;
public:
	void CreateCreepCheckPointByVoxel(const FIntVector& Voxel);
	void InsertLocation(const FIntVector& VoxelIndex);
};
