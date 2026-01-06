// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ChunkGrid.h" // 청크 안에 복셀을 생성하기 위해
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ChunkGenerator.generated.h"

UCLASS()
class COLORDEFENSE_API UChunkGenerator : public UObject
{
	GENERATED_BODY()

public:
	UChunkGrid* ChunkGrid;
	TArray<FIntVector> DirectionContainer;
public:
	UChunkGenerator();
	void Initialize(UChunkGrid* InChunkGrid);
    void GenerateCreepWayChunk(int32 ChunkCount, int32 NeighborRadius);
    TArray<TArray<FIntVector>> GetPatternsUsingDirection(FIntVector Forward);
	FIntVector GetDirectionUsingPattern(TArray<FIntVector> Pattern);
	int32 GetVisitedIndex(const FIntVector& VisitedIndex);
	bool IsSafeToPlace(TArray<int32>& Visited, const FIntVector& TargetPos, int32 CurrentStep, int32 Radius);
};