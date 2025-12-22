// Fill out your copyright notice in the Description page of Project Settings.


#include "CreepGeneratorGenerator.h"

UCreepGeneratorGenerator::UCreepGeneratorGenerator()
{
}

void UCreepGeneratorGenerator::Initialize(UWorld* InWorld, UBPActorPool* InBPActorPool, UChunk* InChunk)
{
    Super::Initialize(InWorld, InBPActorPool, InChunk);
}

void UCreepGeneratorGenerator::CreateCreepGenerator(const FIntVector& VoxelIndex, int32 RailNumber)
{
    SetVoxelDataInChunk(VoxelIndex, 3, EVoxelProperty::CreepGenerator);
    AActor* NewActor = SpawnActorFromVoxel(this->Chunk->GetVoxel(VoxelIndex));

    ACreepGenerator* CreepGenerator = Cast<ACreepGenerator>(NewActor);
    CreepGenerator->SetRailNumber(RailNumber);
    CreepGenerators.Add(CreepGenerator);
}