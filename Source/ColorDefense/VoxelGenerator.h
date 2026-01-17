// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "VoxelGrid.h" // 청크 안에 복셀을 생성하기 위해
#include "BPActorPool.h" // 복셀 속성에 해당하는 BPActor를 Pool에서 바로 가져오기 위해
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "VoxelGenerator.generated.h"

UCLASS()
class COLORDEFENSE_API UVoxelGenerator : public UObject
{
	GENERATED_BODY()

public:
	UWorld* World;
	UBPActorPool* BPActorPool;
	UVoxelGrid* VoxelGrid;
	float VoxelWidth = 250 / 2;
	float VoxelHeight = 125 / 2;
public:
	UVoxelGenerator();
	void Initialize(UWorld* InWorld, UBPActorPool* InBPActorPool, UVoxelGrid* InVoxelGrid, float InVoxelWidth, float InVoxelHeight);
	void SetVoxelDataInVoxelGrid(const FIntVector& VoxelIndex, int32 BPActorPoolIndex, float Rotation, EVoxelProperty Property);
	void DeleteVoxelDataInVoxelGrid(const FIntVector& VoxelIndex);
	AActor* SpawnActorFromVoxel(FVoxel& Voxel);
	void DestroyActorFromVoxel(FVoxel& Voxel);
	FTransform GetWorldTransformFromVoxelIndex(const FIntVector& VoxelIndex, float Width, float Height);
};