// Fill out your copyright notice in the Description page of Project Settings.

#include "CreepEndGeneratorManager.h"

void UCreepEndGeneratorManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UCreepEndGeneratorManager::Deinitialize()
{
    Super::Deinitialize();
}

void UCreepEndGeneratorManager::CreateCreepEndGenerator(UWorld* InWorld, UBPActorPool* InBPActorPool, UVoxelGrid* InVoxelGrid, int32 InMaxRailCount, float InVoxelWidth, float InVoxelHeight)
{
    this->CreepEndGenerator = NewObject<UCreepEndGenerator>(this, UCreepEndGenerator::StaticClass());
    this->CreepEndGenerator->Initialize(InWorld, InBPActorPool, InVoxelGrid, InMaxRailCount, InVoxelWidth, InVoxelHeight);
}