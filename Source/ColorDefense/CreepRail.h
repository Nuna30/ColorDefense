// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Utils/GameEnums.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CreepRail.generated.h"

UCLASS()
class COLORDEFENSE_API UCreepRail : public UObject
{
	GENERATED_BODY()

public:
	UCreepRail();
    void Initialize(int32 InMaxRailCount, int32 InRailLength, FIntVector StartIndex, FIntVector CurrentDirection);
	float GetSlopeCreepWayRotation(const FIntVector& CurrentDirection, const FIntVector& VoxelIndex);
	void UpdateLastIndicesOfEachRail();
	void InsertCreepWayDataRectangleIntoRailBuffers(FIntVector CurrentDirection, int32 BPActorPoolIndex, bool bRotate);
	void InsertCreepWayDataTriangleIntoRailBuffers(bool bTopRailIn, const FIntVector& Direction, int32 Offset);
	void UpdateCreepEnds();

public:
	void PrintLastIndicesOfEachRail();
public:
	int32 RailLength;
	int32 MaxRailCount;
	TArray<TTuple<FIntVector, int32, float, EVoxelProperty>> MainBuffer;
	TArray<TArray<TTuple<FIntVector, int32, float, EVoxelProperty>>> RailBuffers;

	UPROPERTY()
	TArray<FIntVector> LastIndicesOfEachRail;

	UPROPERTY()
	class UCreepEndGeneratorManager* CreepEndGeneratorManager;
};