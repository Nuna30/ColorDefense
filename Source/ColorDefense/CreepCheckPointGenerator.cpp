// Fill out your copyright notice in the Description page of Project Settings.

#include "CreepCheckPointGenerator.h"

UCreepCheckPointGenerator::UCreepCheckPointGenerator()
{
}

void UCreepCheckPointGenerator::Initialize(UWorld* InWorld, UBPActorPool* InBPActorPool, UChunk* InChunk)
{
    Super::Initialize(InWorld, InBPActorPool, InChunk);
}

void UCreepCheckPointGenerator::CreateCreepCheckPointByVoxelIndex(const FIntVector& VoxelIndex)
{
    // CreepCheckPoint는 2칸 차지
    SetVoxelDataInChunk(VoxelIndex, 2, EVoxelProperty::CreepCheckPoint);
    SetVoxelDataInChunk(VoxelIndex + FIntVector(0, 0, 1), 2, EVoxelProperty::CreepCheckPoint);
    SpawnActorFromVoxel(this->Chunk->GetVoxel(VoxelIndex));
    InsertLocation(VoxelIndex);
}

void UCreepCheckPointGenerator::InsertLocation(const FIntVector& VoxelIndex)
{   
    FVector Location = this->Chunk->GetVoxel(VoxelIndex).Transform.GetLocation();
    CreepCheckPointLocations.Add(Location);
}