// Fill out your copyright notice in the Description page of Project Settings.

#include "VoxelGrid.h"

void UVoxelGrid::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UVoxelGrid::InitVoxelGrid()
{
    VoxelGrid.SetNum(VoxelGridSize.X);
    for (int32 X = 0; X < VoxelGridSize.X; X++)
    {
        VoxelGrid[X].SetNum(VoxelGridSize.Y);
        for(int32 Y = 0; Y < VoxelGridSize.Y; Y++)
        {
            VoxelGrid[X][Y].SetNum(VoxelGridSize.Z);
            for(int32 Z = 0; Z < VoxelGridSize.Z; Z++)
            {
                VoxelGrid[X][Y][Z].Index = FIntVector(X, Y, Z);
                VoxelGrid[X][Y][Z] = FVoxel();
            }
        }
    }
}

bool UVoxelGrid::IsInsideVoxelGrid(const FIntVector& VoxelIndex)
{
	int32 x = VoxelIndex.X;
	int32 y = VoxelIndex.Y;
	int32 z = VoxelIndex.Z;

	bool InsideVoxelGrid = true;

	if (!(x >= 0 && x < this->VoxelGridSize.X)) InsideVoxelGrid = false;
	if (!(y >= 0 && y < this->VoxelGridSize.Y)) InsideVoxelGrid = false;
	if (!(z >= 0 && z < this->VoxelGridSize.Z)) InsideVoxelGrid = false;

	return InsideVoxelGrid;
}

bool UVoxelGrid:: IsEmptyIndex(const FIntVector& VoxelIndex)
{
	return this->VoxelGrid[VoxelIndex.X][VoxelIndex.Y][VoxelIndex.Z].Property == EVoxelProperty::Empty;
}

void UVoxelGrid::SetRotation(const FIntVector& VoxelIndex, float Rotation)
{
	FRotator NewRotation = FRotator(0, Rotation, 0);
	this->VoxelGrid[VoxelIndex.X][VoxelIndex.Y][VoxelIndex.Z].Transform.SetRotation(NewRotation.Quaternion());
}

FVoxel& UVoxelGrid::GetVoxel(const FIntVector& VoxelIndex)
{
    if (!IsInsideVoxelGrid(VoxelIndex))
    {
        UE_LOG(LogTemp, Warning, TEXT("You tried to get a Voxel outside the VoxelGrid."));
        return this->VoxelGrid[0][0][0];
    }

    return this->VoxelGrid[VoxelIndex.X][VoxelIndex.Y][VoxelIndex.Z];
}

FIntVector UVoxelGrid::GetCenterIndex()
{
    return FIntVector(this->VoxelGridSize.X / 2, this->VoxelGridSize.Y / 2, this->VoxelGridSize.Z / 2);
}