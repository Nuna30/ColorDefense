// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CreepGenerator.h" // 크립생성기를 생성하기 위해
#include "CoreMinimal.h"
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
