// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CreepGenerator.h"
#include "VoxelGenerator.h"
#include "CreepGeneratorGenerator.generated.h"

UCLASS()
class COLORDEFENSE_API UCreepGeneratorGenerator : public UVoxelGenerator
{
	GENERATED_BODY()
public:
	UCreepGeneratorGenerator();
	void Initialize
	(
		UWorld* InWorld,
		UBPActorPool* InBPActorPool,
		UChunk* InChunk
	);
public:
	void CreateCreepGenerator(const FIntVector& VoxelIndex, int32 RailNumber);
};
