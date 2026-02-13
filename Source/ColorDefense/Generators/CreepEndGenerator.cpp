// Fill out your copyright notice in the Description page of Project Settings.

#include "Data/CreepEnd.h" 
#include "CreepEndGenerator.h"

UCreepEndGenerator::UCreepEndGenerator()
{
}

void UCreepEndGenerator::Initialize(UWorld* InWorld, UBPActorPool* InBPActorPool, UVoxelGrid* InVoxelGrid, int32 InMaxRailCount,  float InVoxelWidth, float InVoxelHeight)
{
    Super::Initialize(InWorld, InBPActorPool, InVoxelGrid, InVoxelWidth, InVoxelHeight);

    this->MaxRailCount = InMaxRailCount;
}

void UCreepEndGenerator::CreateCreepEnds(TArray<FIntVector>& NewLastIndicesOfEachRail)
{
    UE_LOG(LogTemp, Warning, TEXT("Create Creep Ends"));
    for (FIntVector& VoxelIndex : NewLastIndicesOfEachRail)
    {
        VoxelIndex += FIntVector(0, 0, 1);
        SetVoxelDataInVoxelGrid(VoxelIndex, 5, 0, EVoxelProperty::CreepEnd);
        AActor* NewActor = SpawnActorFromVoxel(this->VoxelGrid->GetVoxel(VoxelIndex));

        // I've decided to add null checks to all actor spawning code.
        if (!NewActor)
        {
            UE_LOG(LogTemp, Warning, TEXT("The NewCreepEndActor is NULL in UCreepEndGenerator::CreateCreepEnd."));
            return;
        }

        ACreepEnd* CreepEnd = Cast<ACreepEnd>(NewActor);
        CreepEnds.Add(CreepEnd);
    }
}

void UCreepEndGenerator::MoveCreepEnds(TArray<FIntVector>& NewLastIndicesOfEachRail)
{
    // Delete all old voxel data referring to CurrentCreepEndIndices
    for (const FIntVector& OldIndex : CurrentCreepEndIndices)
    {
        DeleteVoxelDataInVoxelGrid(OldIndex); 
    }

    // Iterate through each CreepEnd to update to its new rail-end position
    for (int32 i = 0; i < CreepEnds.Num(); ++i)
    {
        ACreepEnd* TargetActor = CreepEnds[i];
        FIntVector NewVoxelIndex = NewLastIndicesOfEachRail[i] + FIntVector(0, 0, 1);

        // Set all new index data in the VoxelGrid
        SetVoxelDataInVoxelGrid(NewVoxelIndex, 5, 0.0f, EVoxelProperty::CreepEnd);

        // Update world transforms
        FTransform NewTransform = GetWorldTransformFromVoxelIndex(NewVoxelIndex);
        TargetActor->SetActorLocation(NewTransform.GetLocation());

        // Sync all actors
        FVoxel& NewVoxel = VoxelGrid->VoxelGrid[NewVoxelIndex.X][NewVoxelIndex.Y][NewVoxelIndex.Z];
        NewVoxel.SpawnedActor = TargetActor; 
    }

    // Finally, update our tracking list so the next move knows where the "old" positions are
    CurrentCreepEndIndices = NewLastIndicesOfEachRail;
}