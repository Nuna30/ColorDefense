// Fill out your copyright notice in the Description page of Project Settings.

#include "ChunkGrid.h"

void UChunkGrid::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UChunkGrid::InitChunkGrid()
{
    ChunkGrid.SetNum(ChunkGridSize.X);
    for (int32 X = 0; X < ChunkGridSize.X; X++)
    {
        ChunkGrid[X].SetNum(ChunkGridSize.Y);
        for(int32 Y = 0; Y < ChunkGridSize.Y; Y++)
        {
            ChunkGrid[X][Y].SetNum(ChunkGridSize.Z);
            for(int32 Z = 0; Z < ChunkGridSize.Z; Z++)
            {
                ChunkGrid[X][Y][Z].Index = FIntVector(X, Y, Z);
            }
        }
    }
}

void UChunkGrid::ExpandChunkGrid(const FIntVector& ChunkIndex)
{

}

bool UChunkGrid::IsInsideChunkGrid(const FIntVector& ChunkIndex)
{
	int32 x = ChunkIndex.X;
	int32 y = ChunkIndex.Y;
	int32 z = ChunkIndex.Z;

	bool InsideChunkGrid = true;

	if (!(x >= 0 && x < this->ChunkGridSize.X)) InsideChunkGrid = false;
	if (!(y >= 0 && y < this->ChunkGridSize.Y)) InsideChunkGrid = false;
	if (!(z >= 0 && z < this->ChunkGridSize.Z)) InsideChunkGrid = false;

	return InsideChunkGrid;
}

bool UChunkGrid:: IsEmptyIndex(const FIntVector& ChunkIndex)
{
	return this->ChunkGrid[ChunkIndex.X][ChunkIndex.Y][ChunkIndex.Z].Property == EChunkProperty::Empty;
}

void UChunkGrid:: InsertChunk(const FIntVector& ChunkIndex, EChunkProperty Property)
{
    this->ChunkGrid[ChunkIndex.X][ChunkIndex.Y][ChunkIndex.Z] = FChunk(ChunkIndex, Property);
}