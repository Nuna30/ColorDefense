// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Data/DataStructures/Chunk.h" // Chunk을 청크 안에 저장하기 위해
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ChunkGrid.generated.h"

UCLASS()
class COLORDEFENSE_API UChunkGrid : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	FIntVector ChunkGridSize; // WorldGenerator에서 옵션으로 값 설정됨
	TArray<TArray<TArray<FChunk>>> ChunkGrid;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void InitChunkGrid();
	bool IsInsideChunkGrid(const FIntVector& ChunkIndex);
	bool IsEmptyIndex(const FIntVector& ChunkIndex);
	void InsertChunk(const FIntVector& ChunkIndex, EChunkProperty Property);
};