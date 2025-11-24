// Fill out your copyright notice in the Description page of Project Settings.

#include "CreepCheckPointGenerator.h"
#include "VoxelGenerator.h"

UCreepCheckPointGenerator::UCreepCheckPointGenerator()
{
}

void UCreepCheckPointGenerator::Initialize(UWorld* InWorld, UBPActorPool* InBPActorPool, UChunk* InChunk)
{
    Super::Initialize(InWorld, InBPActorPool, InChunk);
}

void UCreepCheckPointGenerator::CreateCreepCheckPointByVoxel(const FIntVector& VoxelIndex)
{
    SetVoxelDataInChunk(VoxelIndex, 2, EVoxelProperty::CreepCheckPoint);
    SpawnActorFromVoxel(this->Chunk->GetVoxel(VoxelIndex));
    InsertLocation(VoxelIndex);
}

void UCreepCheckPointGenerator::InsertLocation(const FIntVector& VoxelIndex)
{   
    FVector Location = this->Chunk->GetVoxel(VoxelIndex).Transform.GetLocation();
    CreepCheckPointLocations.Add(Location);
}