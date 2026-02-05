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
private:
	int32 NeighborRadius;
	TArray<int32> Visited;
	TArray<FIntVector> ChunkIndexContainer;
public:
	UChunkGenerator();
	void Initialize(UChunkGrid* InChunkGrid, int32 NeighborRadius);
    void GenerateCreepWayChunk(int32 ChunkCount);
	void GenerateNextChunk();
	void GenerateStartLocation();
private:
    TArray<TArray<FIntVector>> GetPatternsUsingDirection(FIntVector Forward);
	FIntVector GetDirectionUsingPattern(TArray<FIntVector> Pattern);
	int32 GetVisitedIndex(const FIntVector& VisitedIndex);
	bool IsSafeToPlace(const FIntVector& TargetPos, int32 CurrentStep, int32 Radius);
};