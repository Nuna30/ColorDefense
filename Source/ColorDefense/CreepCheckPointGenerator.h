// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "VoxelGenerator.h" // CreepCheckPoint Voxel을 생성하기 위해
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CreepCheckPointGenerator.generated.h"

UCLASS()
class COLORDEFENSE_API UCreepCheckPointGenerator : public UVoxelGenerator
{
	GENERATED_BODY()

public:
	UCreepCheckPointGenerator();
	void Initialize (UWorld* InWorld, UBPActorPool* InBPActorPool, UVoxelGrid* InVoxelGrid, float InVoxelWidth, float InVoxelHeight);
	void CreateCreepCheckPointByVoxelIndex(const FIntVector& Voxel);
	void InsertLocation(const FIntVector& VoxelIndex);

public:
	TArray<FVector> CreepCheckPointLocations;
};
