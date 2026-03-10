#pragma once

#include "Data/DataGrids/ChunkGrid.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ChunkGenerator.generated.h"

UCLASS()
class COLORDEFENSE_API UChunkGenerator : public UObject
{
	GENERATED_BODY()

public: // --- Initialization --- //
	void Initialize(UChunkGrid* InChunkGrid, int32 NeighborRadius);
	void GenerateStartingPoint();

public: // --- Delegate --- //
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChunkGenerated);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChunkDeleted);

	UPROPERTY()
	FOnChunkGenerated OnChunkGenerated;

	UPROPERTY()
	FOnChunkDeleted OnChunkDeleted;

public: // --- Components --- //
	UChunkGrid* ChunkGrid;
	TArray<int32> Visited;
	TArray<FIntVector> ChunkIndexContainer;
	
public: // --- Dynamic Chunk Generation --- //
	int32 ChunkCount;
	int32 NeighborRadius;
	TArray<FIntVector> DirectionContainer;
	bool GenerateNextChunk();
	void DeleteCurrentChunk();

public: // --- Pattern Generation --- //
	TArray<TArray<FIntVector>> GetPatternsUsingDirection(FIntVector Forward);

public: // --- Utils --- //
	FIntVector GetDirectionUsingPattern(TArray<FIntVector> Pattern);
	int32 GetVisitedIndex(const FIntVector& VisitedIndex);
	bool IsSafeToPlace(const FIntVector& TargetPos, int32 CurrentStep, int32 Radius);
};