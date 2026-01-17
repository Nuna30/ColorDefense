// Fill out your copyright notice in the Description page of Project Settings.

#include "CreepCheckPointGenerator.h"

UCreepCheckPointGenerator::UCreepCheckPointGenerator()
{
}

void UCreepCheckPointGenerator::Initialize(UWorld* InWorld, UBPActorPool* InBPActorPool, UVoxelGrid* InVoxelGrid, float InVoxelWidth, float InVoxelHeight)
{
    Super::Initialize(InWorld, InBPActorPool, InVoxelGrid, InVoxelWidth, InVoxelHeight);
}

void UCreepCheckPointGenerator::CreateCreepCheckPointByVoxelIndex(const FIntVector& VoxelIndex)
{
    // CreepCheckPoint는 2칸 차지
    SetVoxelDataInVoxelGrid(VoxelIndex, 2, 0, EVoxelProperty::CreepCheckPoint);
    SetVoxelDataInVoxelGrid(VoxelIndex + FIntVector(0, 0, 1), 2, 0, EVoxelProperty::CreepCheckPoint);
    SpawnActorFromVoxel(this->VoxelGrid->GetVoxel(VoxelIndex));
    InsertLocation(VoxelIndex);
}

void UCreepCheckPointGenerator::InsertLocation(const FIntVector& VoxelIndex)
{   
    FVector Location = this->VoxelGrid->GetVoxel(VoxelIndex).Transform.GetLocation();
    this->CreepCheckPointLocations.Add(Location);
}