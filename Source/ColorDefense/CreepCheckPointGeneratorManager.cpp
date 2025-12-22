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

void UCreepCheckPointGeneratorManager::CreateCreepCheckPointGenerators(UWorld* InWorld, UBPActorPool* InBPActorPool, UChunk* InChunk, int32 InMaxRailCount)
{
    this->MaxRailCount = InMaxRailCount;
    for (int32 i = 0; i < this->MaxRailCount; i++)
    {
        UCreepCheckPointGenerator* NewCreepCheckPointGenerator = NewObject<UCreepCheckPointGenerator>(this, UCreepCheckPointGenerator::StaticClass());
        NewCreepCheckPointGenerator->Initialize(InWorld, InBPActorPool, InChunk);
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