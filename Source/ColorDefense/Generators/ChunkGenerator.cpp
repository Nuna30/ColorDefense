#include "ChunkGenerator.h"

// ========================== //
// ===== Initialization ===== //
// ========================== //

void UChunkGenerator::Initialize(UChunkGrid* InChunkGrid, int32 InNeighborRadius)
{
    // The Reason the goat climbs the mountain is its stubbornness.
    this->ChunkCount = 2;
    this->ChunkGrid = InChunkGrid;
    this->NeighborRadius = InNeighborRadius;

    // Generate two chunk.
    GenerateStartingPoint();
}


void UChunkGenerator::GenerateStartingPoint()
{
    // Calculate the boundary of ChunkGrid and initialize Visited using it.
    int p = this->ChunkGrid->ChunkGrid.Num();
    int q = this->ChunkGrid->ChunkGrid[0].Num();
    int r = this->ChunkGrid->ChunkGrid[0][0].Num();
    Visited.Init(0, p * q * r);

    // Define the starting direction and index.
    FIntVector StartingDirection = FIntVector(1, 0, 0);
    FIntVector StartingIndex = FIntVector(p / 2, q / 2, r / 2);

    // Generate the starting point.
    Visited[GetVisitedIndex(StartingIndex)] = 1;
    ChunkIndexContainer.Push(StartingIndex);
    StartingIndex += StartingDirection;
    Visited[GetVisitedIndex(StartingIndex)] = 2;
    ChunkIndexContainer.Push(StartingIndex);
    this->DirectionContainer.Add(StartingDirection);

    GenerateNextChunk();
}

// ==================================== //
// ===== Dynamic Chunk Generation ===== //
// ==================================== //

void UChunkGenerator::DeleteCurrentChunk()
{
    // Decrement ChunkCount by 3.
    if (ChunkCount <= 2) return;
    ChunkCount -= 3; 

    // Remove exactly 3 chunks.
    for (int32 i = 0; i < 3; i++)
    {
        // Remove a chunk.
        FIntVector RemovePos = ChunkIndexContainer.Pop();
        Visited[GetVisitedIndex(RemovePos)] = 0;
        this->ChunkGrid->InsertChunk(RemovePos, EChunkProperty::Empty);
    }

    // Remove the last direction.
    DirectionContainer.Pop();

    // Broadcast the chunk update.
    OnChunkDeleted.Broadcast();
}

bool UChunkGenerator::GenerateNextChunk()
{
    // Generate a Pattern referring to the last direciton.
    FIntVector LastDirection = this->DirectionContainer.Last();
    TArray<TArray<FIntVector>> Patterns = GetPatternsUsingDirection(LastDirection);

    // Use Fisher-Yates Shuffle to shuffle the Pattern array.
    for (int32 i = Patterns.Num() - 1; i > 0; i--)
    {
        int32 j = FMath::RandRange(0, i);
        Patterns.Swap(i, j);
    }

    // Simulate all patterns to find a safe pattern.
    int32 SafePatternIdx = -1;
    bool bInsideChunkGrid = false;
    bool bSafeToPlace = false;
    for (int32 i = 0; i < Patterns.Num(); i++)
    {
        // Simulate a pattern to validate it.
        FIntVector CheckPos = ChunkIndexContainer.Last();
        int32 TempStep = ChunkIndexContainer.Num() + 1;
        TArray<FIntVector> Pattern = Patterns[i];
        int32 StepCount = 0;
        for (const FIntVector& Step : Pattern)
        {
            CheckPos += Step;
            if (!this->ChunkGrid->IsInsideChunkGrid(CheckPos)) break;
            if (!IsSafeToPlace(CheckPos, TempStep++, NeighborRadius)) break;
            StepCount++;
        }

        // If the pattern passes all checks, it is safe.
        if (StepCount == Pattern.Num()) SafePatternIdx = i;
    }

    // If the SafePatternIdx has no value, that means there is nowhere to go...
    if (SafePatternIdx == -1) return false;

    // Use the safe pattern to generate next chunk.
    FIntVector CurrentPos = ChunkIndexContainer.Last();
    TArray<FIntVector> Pattern = Patterns[SafePatternIdx];
    for (const FIntVector& Step : Pattern)
    {
        CurrentPos += Step;
        Visited[GetVisitedIndex(CurrentPos)] = ChunkIndexContainer.Num() + 1;
        ChunkIndexContainer.Push(CurrentPos);
    }

    // Update the direction and ChunkCount.
    this->DirectionContainer.Add(GetDirectionUsingPattern(Pattern));
    ChunkCount += 3;

    // Broadcast the chunk update.
    OnChunkGenerated.Broadcast();

    return true;
}

// ============================== //
// ===== Pattern Generation ===== //
// ============================== //

TArray<TArray<FIntVector>> UChunkGenerator::GetPatternsUsingDirection(FIntVector Direction)
{
    // The Direction needs to be flattened.
    Direction.Z = 0; 

    // Calculate the Right Vector using cross product.
    FVector FwdVec = FVector(Direction.X, Direction.Y, Direction.Z);
    FVector UpVec = FVector(0, 0, 1);
    FVector RightVec = FVector::CrossProduct(FwdVec, UpVec);

    // Transform to IntVector.
    FIntVector Up = FIntVector(0, 0, 1);
    FIntVector Right = FIntVector(RightVec.X, RightVec.Y, RightVec.Z);

    // Generate Patterns combining Direction, Up and Right.
    TArray<TArray<FIntVector>> ResultPatterns;
    ResultPatterns.Add({Direction, Direction + Up, Direction });
    ResultPatterns.Add({Direction, Direction - Up, Direction });
    ResultPatterns.Add({Direction, Direction, Right });
    ResultPatterns.Add({Direction, Direction, Right * -1 });

    return ResultPatterns;
}

// ================= //
// ===== Utils ===== //
// ================= //

FIntVector UChunkGenerator::GetDirectionUsingPattern(TArray<FIntVector> Pattern)
{
    if (Pattern[1].Z == 0) return Pattern[2];
    else return Pattern[1];
}

bool UChunkGenerator::IsSafeToPlace(const FIntVector& TargetPos, int32 CurrentStep, int32 Radius)
{
    // Skip if already occupied.
    if (Visited[GetVisitedIndex(TargetPos)] != 0) return false;

    // --- !!Core concept!! --- //
    // Head : The most recently generated chunk (the "front" of the path)
    // Neck : Just-generated
    // Tail : generated long ago (Not safe)

    // Search surrounding area for a "Tail" chunk.
    for (int i = -Radius; i <= Radius; i++)
    {
        for (int j = -Radius; j <= Radius; j++)
        {
            for (int k = -Radius; k <= Radius; k++)
            {
                // Neighbors to check.
                FIntVector NeighborPos = TargetPos + FIntVector(i, j, k);

                // Ignore if outside the Chunkgrid.
                if (!this->ChunkGrid->IsInsideChunkGrid(NeighborPos)) continue;

                // Smaller step means Older chunk.
                int32 NeighborStep = Visited[GetVisitedIndex(NeighborPos)];

                // If empty, it's safe.
                if (NeighborStep == 0) continue;

                // --- Core logic --- //
                // Is it Tail or Neck?
                if (FMath::Abs(CurrentStep - NeighborStep) > Radius + 3)
                {
                    // This chunk is "Tail".
                    return false;
                }
            }
        }
    }

    return true; // Empty or "Neck" only (Safe)
}

int32 UChunkGenerator::GetVisitedIndex(const FIntVector& VisitedIndex)
{
    int p = this->ChunkGrid->ChunkGrid.Num();
    int q = this->ChunkGrid->ChunkGrid[0].Num();
    int r = this->ChunkGrid->ChunkGrid[0][0].Num();
    return VisitedIndex.X + (VisitedIndex.Y * p) + (VisitedIndex.Z * p * q);
}