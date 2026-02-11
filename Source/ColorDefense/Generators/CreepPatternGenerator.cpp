// Fill out your copyright notice in the Description page of Project Settings.

#include "CreepPatternGenerator.h"

UCreepPatternGenerator::UCreepPatternGenerator()
{
    
}

void UCreepPatternGenerator::Initialize(int32 InMaxRailCount)
{
    this->MaxRailCount = InMaxRailCount;
    this->PatternArray.SetNum(MaxRailCount);
}

bool UCreepPatternGenerator::IsEmpty(int32 RailNumber)
{
    return this->PatternArray[RailNumber].IsEmpty();
}

FSpawnInfo UCreepPatternGenerator::PopSpawnInfo(int32 RailNumber)
{
    FSpawnInfo SpawnInfo = this->PatternArray[RailNumber][0];
    this->PatternArray[RailNumber].RemoveAt(0);
    return SpawnInfo;
}

void UCreepPatternGenerator::GeneratePattern()
{
    // 1. Setup the parameters
    int32 RailCount = this->MaxRailCount;
    int32 BackgroundCreepColorNumber = FMath::RandRange(1, 5);
    int32 Row = FMath::RandRange(0, this->MaxRailCount - 1);
    int32 Col = FMath::RandRange(0, this->PatternArray[Row].Num() - 1);
    int32 NewCreepColorNumber = FMath::RandRange(1, 5);
    int32 PropagationCount = this->MaxRailCount * this->MaxRailCount / 3;

    // 2. Generate Pattern
    FillPatternArray(RailCount, BackgroundCreepColorNumber);
    ChangeCreepColorNumberUsingDFS(Row, Col, NewCreepColorNumber, PropagationCount);
}

void UCreepPatternGenerator::FillPatternArray(int32 Count, int32 CreepColorNumber)
{
    for (int32 i = 0; i < this->PatternArray.Num(); i++)
    {
        for(int32 j = 0; j < Count; j++)
        {
            this->PatternArray[i].Add(FSpawnInfo(CreepColorNumber, this->SpawnDelay));
        }
    }
}

void UCreepPatternGenerator::ChangeCreepColorNumberUsingDFS(int32 Row, int32 Col, int32 NewCreepColorNumber, int32 PropagationCount)
{
    // 1. Setup DFS Containers (Using TArray as a Stack)
    TArray<FIntPoint> Stack;
    TSet<FIntPoint> Visited;

    FIntPoint StartNode(Row, Col);
    Stack.Push(StartNode);
    // Note: We don't mark Visited here yet to count it inside the loop
    
    int32 CurrentCount = 0;

    // 2. Direction offsets for neighbors (Up, Down, Left, Right)
    TArray<FIntPoint> Directions = {FIntPoint(-1, 0), FIntPoint(1, 0), FIntPoint(0, -1), FIntPoint(0, 1)};

    // 3. DFS Loop
    while (Stack.Num() > 0 && CurrentCount < PropagationCount)
    {
        FIntPoint Current = Stack.Pop();

        // Check visited here to handle the stack-based DFS correctly
        if (Visited.Contains(Current)) continue;

        // Apply change and mark as visited
        PatternArray[Current.X][Current.Y].CreepColorNumber = NewCreepColorNumber;
        Visited.Add(Current);
        CurrentCount++;

        // Shuffle directions randomly for each node
        for (int32 i = Directions.Num() - 1; i > 0; --i)
        {
            int32 j = FMath::RandRange(0, i);
            Directions.Swap(i, j);
        }

        // Add neighbors to stack
        for (const FIntPoint& Dir : Directions)
        {
            FIntPoint Neighbor = Current + Dir;

            // Boundary Check
            if (!PatternArray.IsValidIndex(Neighbor.X)) continue;
            if (!PatternArray[Neighbor.X].IsValidIndex(Neighbor.Y)) continue;
            
            // If not visited, add to stack to explore deeper
            if (!Visited.Contains(Neighbor))
            {
                Stack.Push(Neighbor);
            }
        }
    }
}

void UCreepPatternGenerator::ChangeCreepColorNumberUsingBFS(int32 Row, int32 Col, int32 NewCreepColorNumber, int32 Prob)
{
    // 1. Setup BFS Containers
    TQueue<FIntPoint> Frontier;
    TSet<FIntPoint> Visited;

    FIntPoint StartNode(Row, Col);
    Frontier.Enqueue(StartNode);
    Visited.Add(StartNode);

    // Change the starting node color immediately
    PatternArray[Row][Col].CreepColorNumber = NewCreepColorNumber;

    // 2. Direction offsets for neighbors (Up, Down, Left, Right)
    TArray<FIntPoint> Directions = {FIntPoint(-1, 0), FIntPoint(1, 0), FIntPoint(0, -1), FIntPoint(0, 1)};

    // 3. BFS Loop
    while (!Frontier.IsEmpty())
    {
        FIntPoint Current;
        Frontier.Dequeue(Current);

        for (const FIntPoint& Dir : Directions)
        {
            FIntPoint Neighbor = Current + Dir;

            // Boundary Check
            if (PatternArray.IsValidIndex(Neighbor.X) == false) continue;
            if (PatternArray[Neighbor.X].IsValidIndex(Neighbor.Y) == false) continue;
            // Visited Check
            if (Visited.Contains(Neighbor)) continue;
            Visited.Add(Neighbor);
            // Probability roll (Prob is 0-100)
            if (FMath::RandRange(1, 100) > Prob) continue;
            // Apply change and add to queue to propagate further
            PatternArray[Neighbor.X][Neighbor.Y].CreepColorNumber = NewCreepColorNumber;
            Frontier.Enqueue(Neighbor);
        }
    }
}