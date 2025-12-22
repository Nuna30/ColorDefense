// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Voxel.h" // Voxel을 청크 안에 저장하기 위해
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Chunk.generated.h"

UCLASS()
class COLORDEFENSE_API UChunk : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	FIntVector ChunkSize = FIntVector(400, 400, 200); // 400 x 400 x 200 = 32,000,000
	TArray<TArray<TArray<FVoxel>>> Chunk; // Voxel이 80바이트 정도면 32,000,000 x 80 = 2,560,000,000 (2.5 기가 ㄷㄷ...)

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void ExpandChunk(const FIntVector& VoxelIndex);
	bool IsInsideChunk(const FIntVector& VoxelIndex);
	bool IsEmptyIndex(const FIntVector& VoxelIndex);
	void SetRotation(const FIntVector& VoxelIndex, float Rotation);
	FVoxel& GetVoxel(const FIntVector& VoxelIndex);
};