// Fill out your copyright notice in the Description page of Project Settings.

#include "CreepGeneratorGeneratorManager.h"

void UCreepGeneratorGeneratorManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UCreepGeneratorGeneratorManager::Deinitialize()
{
    this->ClearCreepGeneratorGenerator();
    Super::Deinitialize();
}

void UCreepGeneratorGeneratorManager::CreateCreepGeneratorGenerator(UWorld* InWorld, UBPActorPool* InBPActorPool, UVoxelGrid* InVoxelGrid)
{
    this->CreepGeneratorGenerator = NewObject<UCreepGeneratorGenerator>(this, UCreepGeneratorGenerator::StaticClass());
    this->CreepGeneratorGenerator->Initialize(InWorld, InBPActorPool, InVoxelGrid);
}

void UCreepGeneratorGeneratorManager::ClearCreepGeneratorGenerator()
{
    if (this->CreepGeneratorGenerator)
        this->CreepGeneratorGenerator = nullptr;
}