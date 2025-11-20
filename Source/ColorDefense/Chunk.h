// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Voxel.h"
#include "Chunk.generated.h"

struct FVoxel;

UCLASS()
class COLORDEFENSE_API UChunk : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void ExpandChunk(const FIntVector& VoxelIndex);
	bool IsInsideChunk(const FIntVector& VoxelIndex);
	bool IsEmptyIndex(const FIntVector& VoxelIndex);
	void SetRotation(const FIntVector& VoxelIndex, float Rotation);
public:
	FIntVector ChunkSize = FIntVector(200, 200, 100); // 400 x 400 x 200 = 32,000,000
	TArray<TArray<TArray<FVoxel>>> Chunk; // Voxel이 80바이트 정도면 32,000,000 x 80 = 2,560,000,000 (2.5 기가 ㄷㄷ...)
};
