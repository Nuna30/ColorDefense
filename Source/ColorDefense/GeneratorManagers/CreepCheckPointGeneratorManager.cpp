// Fill out your copyright notice in the Description page of Project Settings.

#include "CreepCheckPointGeneratorManager.h"

void UCreepCheckPointGeneratorManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    this->MaxRailCount = 0;
}

void UCreepCheckPointGeneratorManager::Deinitialize()
{
    this->ClearAllCreepCheckPointGenerators();
    Super::Deinitialize();
}

void UCreepCheckPointGeneratorManager::CreateCreepCheckPointGenerators(UWorld* InWorld, UBPActorPool* InBPActorPool, UVoxelGrid* InVoxelGrid, int32 InMaxRailCount, float InVoxelWidth, float InVoxelHeight, bool bShowCreepCheckPoints)
{
    this->MaxRailCount = InMaxRailCount;
    for (int32 i = 0; i < this->MaxRailCount; i++)
    {
        UCreepCheckPointGenerator* NewCreepCheckPointGenerator = NewObject<UCreepCheckPointGenerator>(this, UCreepCheckPointGenerator::StaticClass());
        NewCreepCheckPointGenerator->Initialize(InWorld, InBPActorPool, InVoxelGrid, InVoxelWidth, InVoxelHeight, bShowCreepCheckPoints);
        CreepCheckPointGenerators.Add(NewCreepCheckPointGenerator);
    }
}

void UCreepCheckPointGeneratorManager::ClearAllCreepCheckPointGenerators()
{
    for (int32 i = 0; i < this->CreepCheckPointGenerators.Num(); i++)
    {
        if (this->CreepCheckPointGenerators[i])
            this->CreepCheckPointGenerators[i] = nullptr;
    }
}