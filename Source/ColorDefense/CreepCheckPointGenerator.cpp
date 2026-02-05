// Fill out your copyright notice in the Description page of Project Settings.

#include "CreepCheckPointGenerator.h"

UCreepCheckPointGenerator::UCreepCheckPointGenerator()
{
}

void UCreepCheckPointGenerator::Initialize(UWorld* InWorld, UBPActorPool* InBPActorPool, UVoxelGrid* InVoxelGrid, float InVoxelWidth, float InVoxelHeight)
{
    Super::Initialize(InWorld, InBPActorPool, InVoxelGrid, InVoxelWidth, InVoxelHeight);
}

AActor* UCreepCheckPointGenerator::CreateCreepCheckPointByVoxelIndex(const FIntVector& VoxelIndex)
{
    // Set CreepCheckPoint voxel.
    SetVoxelDataInVoxelGrid(VoxelIndex, 2, 0, EVoxelProperty::CreepCheckPoint);

    // Capture and return the spawned actor
    AActor* SpawnedCP = SpawnActorFromVoxel(this->VoxelGrid->GetVoxel(VoxelIndex));
    InsertLocation(VoxelIndex);
    return SpawnedCP;
}

void UCreepCheckPointGenerator::InsertLocation(const FIntVector& VoxelIndex)
{   
    FVector Location = this->VoxelGrid->GetVoxel(VoxelIndex).Transform.GetLocation();
    this->CreepCheckPointLocations.Add(Location);
}

void UCreepCheckPointGenerator::PopLocations(int32 Count)
{
    while (Count > 0 && CreepCheckPointLocations.Num() > 0)
    {
        CreepCheckPointLocations.Pop();
        Count--;
    }
}