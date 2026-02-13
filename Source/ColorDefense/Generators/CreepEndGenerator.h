// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "VoxelGenerator.h"
#include "CoreMinimal.h"
#include "CreepEndGenerator.generated.h"

UCLASS()
class COLORDEFENSE_API UCreepEndGenerator : public UVoxelGenerator
{
	GENERATED_BODY()

public:
	int32 MaxRailCount;

	UPROPERTY()
	TArray<class ACreepEnd*> CreepEnds;

	UPROPERTY()
	TArray<FIntVector> CurrentCreepEndIndices;
public:
	UCreepEndGenerator();
	void Initialize
	(
		UWorld* InWorld,
		UBPActorPool* InBPActorPool,
		UVoxelGrid* InVoxelGrid,
		int32 InMaxRailCount, 
		float InVoxelWidth, 
		float InVoxelHeight
	);
	void CreateCreepEnds(TArray<FIntVector>& NewLastIndicesOfEachRail);
	void MoveCreepEnds(TArray<FIntVector>& NewLastIndicesOfEachRail);
};