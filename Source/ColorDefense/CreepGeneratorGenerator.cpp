// Fill out your copyright notice in the Description page of Project Settings.

#include "CreepGeneratorGenerator.h"

UCreepGeneratorGenerator::UCreepGeneratorGenerator()
{
}

void UCreepGeneratorGenerator::Initialize(UWorld* InWorld, UBPActorPool* InBPActorPool, UVoxelGrid* InVoxelGrid, float InVoxelWidth, float InVoxelHeight)
{
    Super::Initialize(InWorld, InBPActorPool, InVoxelGrid, InVoxelWidth, InVoxelHeight);
}

void UCreepGeneratorGenerator::CreateCreepGenerator(const FIntVector& VoxelIndex, int32 RailNumber)
{
    SetVoxelDataInVoxelGrid(VoxelIndex, 3, 0, EVoxelProperty::CreepGenerator);
    AActor* NewActor = SpawnActorFromVoxel(this->VoxelGrid->GetVoxel(VoxelIndex));

    // I've decided to add null checks to all actor spawning code.
    if (!NewActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("The NewCreepActor is NULL in UCreepGeneratorGenerator::CreateCreepGenerator."));
        return;
    }

    ACreepGenerator* CreepGenerator = Cast<ACreepGenerator>(NewActor);
    CreepGenerator->SetRailNumber(RailNumber);
    CreepGenerators.Add(CreepGenerator);
}

void UCreepGeneratorGenerator::StartCreepGeneration(float PrepTime)
{
	FTimerHandle Timer;
    GetWorld()->GetTimerManager().SetTimer(
		Timer,         // 타이머 핸들
		this,                // 실행할 대상 객체
		&UCreepGeneratorGenerator::OperateCreepGenerators, // 실행할 함수 포인터
		PrepTime,                // 주기 (초 단위)
		false                 // 반복 여부 (true면 무한 반복)
	);
}

void UCreepGeneratorGenerator::OperateCreepGenerators()
{   
    for (ACreepGenerator* CreepGenerator : CreepGenerators)
    {
        CreepGenerator->GenerateCreeps();
    }
}