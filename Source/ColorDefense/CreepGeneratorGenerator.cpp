// Fill out your copyright notice in the Description page of Project Settings.


#include "CreepGeneratorGenerator.h"

UCreepGeneratorGenerator::UCreepGeneratorGenerator()
{
}

void UCreepGeneratorGenerator::Initialize(UWorld* InWorld, UBPActorPool* InBPActorPool, UVoxelGrid* InVoxelGrid)
{
    Super::Initialize(InWorld, InBPActorPool, InVoxelGrid);
}

void UCreepGeneratorGenerator::CreateCreepGenerator(const FIntVector& VoxelIndex, int32 RailNumber)
{
    SetVoxelDataInVoxelGrid(VoxelIndex, 3, EVoxelProperty::CreepGenerator);
    AActor* NewActor = SpawnActorFromVoxel(this->VoxelGrid->GetVoxel(VoxelIndex));

    ACreepGenerator* CreepGenerator = Cast<ACreepGenerator>(NewActor);
    CreepGenerator->SetRailNumber(RailNumber);
    CreepGenerators.Add(CreepGenerator);
}