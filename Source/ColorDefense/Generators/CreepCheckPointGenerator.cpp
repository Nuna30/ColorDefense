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
    if (!this->VoxelGrid->IsInsideVoxelGrid(VoxelIndex))
    {
        UE_LOG(LogTemp, Warning, TEXT("The CreepCheckPoint tried to accessed to the outside voxel grid."));
        UE_LOG(LogTemp, Warning, TEXT("The CreepCheckPoint index : %d %d %d."), VoxelIndex.X, VoxelIndex.Y, VoxelIndex.Z);
        return nullptr;
    }

    // Set CreepCheckPoint voxel.
    SetVoxelDataInVoxelGrid(VoxelIndex, 2, 0, EVoxelProperty::CreepCheckPoint);

    // Capture and return the spawned actor
    AActor* SpawnedCP = SpawnActorFromVoxel(this->VoxelGrid->GetVoxel(VoxelIndex));

    // I've decided to add null checks to all actor spawning code.
    if (!SpawnedCP)
    {
        UE_LOG(LogTemp, Warning, TEXT("The SpawnedCP is NULL in  UCreepCheckPointGenerator::CreateCreepCheckPointByVoxelIndex."));
        return nullptr;
    }

    InsertLocation(VoxelIndex);
    return SpawnedCP;
}

void UCreepCheckPointGenerator::InsertLocation(const FIntVector& VoxelIndex)
{   
    FVector Location = this->VoxelGrid->GetVoxel(VoxelIndex).Transform.GetLocation();
    Location += FVector(0, 0, Super::VoxelHeight);
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