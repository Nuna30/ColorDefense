// Fill out your copyright notice in the Description page of Project Settings.

#include "Chunk.h"

void UChunk::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    Chunk.SetNum(ChunkSize.X);
    for (int32 X = 0; X < ChunkSize.X; X++)
    {
        Chunk[X].SetNum(ChunkSize.Y);
        for(int32 Y = 0; Y < ChunkSize.Y; Y++)
        {
            Chunk[X][Y].SetNum(ChunkSize.Z);
            for(int32 Z = 0; Z < ChunkSize.Z; Z++)
            {
                Chunk[X][Y][Z].Index = FIntVector(X, Y, Z);
            }
        }
    }
}

void UChunk::ExpandChunk(const FIntVector& VoxelIndex)
{

}

bool UChunk::IsInsideChunk(const FIntVector& VoxelIndex)
{
	int32 x = VoxelIndex.X;
	int32 y = VoxelIndex.Y;
	int32 z = VoxelIndex.Z;

	bool InsideChunk = true;

	if (!(x >= 0 && x < this->ChunkSize.X)) InsideChunk = false;
	if (!(y >= 0 && y < this->ChunkSize.Y)) InsideChunk = false;
	if (!(z >= 0 && z < this->ChunkSize.Z)) InsideChunk = false;

	return InsideChunk;
}

bool UChunk:: IsEmptyIndex(const FIntVector& VoxelIndex)
{
	return this->Chunk[VoxelIndex.X][VoxelIndex.Y][VoxelIndex.Z].Property == EVoxelProperty::Empty;
}

void UChunk::SetRotation(const FIntVector& VoxelIndex, float Rotation)
{
	FRotator NewRotation = FRotator(0, Rotation, 0);
	this->Chunk[VoxelIndex.X][VoxelIndex.Y][VoxelIndex.Z].Transform.SetRotation(NewRotation.Quaternion());
}

FVoxel& UChunk::GetVoxel(const FIntVector& VoxelIndex)
{
    return this->Chunk[VoxelIndex.X][VoxelIndex.Y][VoxelIndex.Z];
}