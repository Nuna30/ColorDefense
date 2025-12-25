// Fill out your copyright notice in the Description page of Project Settings.

#include "CreepCheckPointGenerator.h"

UCreepCheckPointGenerator::UCreepCheckPointGenerator()
{
}

void UCreepCheckPointGenerator::Initialize(UWorld* InWorld, UBPActorPool* InBPActorPool, UVoxelGrid* InVoxelGrid)
{
    Super::Initialize(InWorld, InBPActorPool, InVoxelGrid);
}

void UCreepCheckPointGenerator::CreateCreepCheckPointByVoxelIndex(const FIntVector& VoxelIndex)
{
    // CreepCheckPoint는 2칸 차지
    SetVoxelDataInVoxelGrid(VoxelIndex, 2, EVoxelProperty::CreepCheckPoint);
    SetVoxelDataInVoxelGrid(VoxelIndex + FIntVector(0, 0, 1), 2, EVoxelProperty::CreepCheckPoint);
    SpawnActorFromVoxel(this->VoxelGrid->GetVoxel(VoxelIndex));
    InsertLocation(VoxelIndex);
}

void UCreepCheckPointGenerator::InsertLocation(const FIntVector& VoxelIndex)
{   
    FVector Location = this->VoxelGrid->GetVoxel(VoxelIndex).Transform.GetLocation();
    this->CreepCheckPointLocations.Add(Location);
}