// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Voxel.h" // Voxel을 청크 안에 저장하기 위해
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "VoxelGrid.generated.h"

UCLASS()
class COLORDEFENSE_API UVoxelGrid : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	FIntVector VoxelGridSize; // WorldGenerator에서 옵션으로 값 설정됨
	TArray<TArray<TArray<FVoxel>>> VoxelGrid;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void InitVoxelGrid();
	bool IsInsideVoxelGrid(const FIntVector& VoxelIndex);
	bool IsEmptyIndex(const FIntVector& VoxelIndex);
	void SetRotation(const FIntVector& VoxelIndex, float Rotation);
	FVoxel& GetVoxel(const FIntVector& VoxelIndex);
	FIntVector GetCenterIndex();
};