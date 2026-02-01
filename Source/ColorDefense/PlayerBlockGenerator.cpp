// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils.h"
#include "PlayerBlockGenerator.h"
#include "CreepRail.h"

#define DEBUGMODE 1

UPlayerBlockGenerator::UPlayerBlockGenerator()
{
}

void UPlayerBlockGenerator::Initialize
(
	UWorld* InWorld, 
	UBPActorPool* InBPActorPool, 
	UVoxelGrid* InVoxelGrid, 
	float InVoxelWidth, 
	float InVoxelHeight
)
{
    Super::Initialize(InWorld, InBPActorPool, InVoxelGrid, InVoxelWidth, InVoxelHeight);
}

void UPlayerBlockGenerator::GeneratePlayerBlock()
{
	for (const FIntVector& PlayerBlockIndex : PlayerBlockBuffer)
	{
		SpawnInvisibleNeighboringPlaceables(PlayerBlockIndex);
	}
}

void UPlayerBlockGenerator::SpawnInvisibleNeighboringPlaceables(const FIntVector& VoxelIndex)
{
	// Neighbors where Playerblock can be placed
	TArray<FIntVector> Neighbor4 = 
	{
		FIntVector(1, 0, 0), FIntVector(-1, 0, 0), FIntVector(0, 1, 0), FIntVector(0, -1, 0), FIntVector(0, 0, 1)
	};

	// Set Neighbors to Placeable and spawn as invisible
	for (FIntVector& Neighbor : Neighbor4)
	{
		Neighbor += VoxelIndex;
		FVoxel& TargetVoxel = VoxelGrid->GetVoxel(Neighbor);
		
		if (!VoxelGrid->IsInsideVoxelGrid(Neighbor)) continue;
		if (TargetVoxel.Property != EVoxelProperty::Empty) continue;
		
		// PlayerBlock can't be placed above or below the SlopeCreepWay
		FVoxel SlopeVoxel = this->VoxelGrid->GetVoxel(Neighbor + FIntVector(0, 0, -1));
		if (SlopeVoxel.Property == EVoxelProperty::SlopeCreepWay) continue;
		SlopeVoxel = this->VoxelGrid->GetVoxel(Neighbor + FIntVector(0, 0, 1));
		if (SlopeVoxel.Property == EVoxelProperty::SlopeCreepWay) continue;

		// PlayerBlock can't be placed above the NormalCreepWay
		FVoxel BelowVoxel = this->VoxelGrid->GetVoxel(Neighbor + FIntVector(0, 0, -1));
		if (BelowVoxel.Property == EVoxelProperty::NormalCreepWay) continue;

		// Spawn the PlayerBlock
		SetVoxelDataInVoxelGrid(Neighbor, 4, 0, EVoxelProperty::PlayerBlock);
		AActor* Placeable = SpawnActorFromVoxel(TargetVoxel);

		// Configure the PlayerBlock
		Placeable->SetActorHiddenInGame(true);
		APlayerBlock* PlayerBlock = Cast<APlayerBlock>(Placeable);
		PlayerBlock->Voxel = TargetVoxel;
		PlayerBlock->SetOpacity(0.1);
		PlayerBlock->SetCollisionWithPawn(false);
	}
}

void UPlayerBlockGenerator::Add(const FIntVector& PlayerBlockIndex)
{
	this->PlayerBlockBuffer.Add(PlayerBlockIndex);
}