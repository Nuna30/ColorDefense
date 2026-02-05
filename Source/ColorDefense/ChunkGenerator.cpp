// Fill out your copyright notice in the Description page of Project Settings.

#include "ChunkGenerator.h"

#define DEBUGMODE 1

UChunkGenerator::UChunkGenerator()
{
}

void UChunkGenerator::Initialize(UChunkGrid* InChunkGrid, int32 InNeighborRadius)
{
    // The Reason the goat climbs the mountain is its stubbornness.
    this->ChunkGrid = InChunkGrid;
    this->NeighborRadius = InNeighborRadius;
}

TArray<TArray<FIntVector>> UChunkGenerator::GetPatternsUsingDirection(FIntVector Direction)
{
    // 1. 기준이 되는 '위쪽(Up)'과 '오른쪽(Right)' 벡터 구하기
    FVector FwdVec = FVector(Direction.X, Direction.Y, Direction.Z);
    FVector UpVec = FVector(0, 0, 1);
    FVector RightVec = FVector::CrossProduct(UpVec, FwdVec);

    // 다시 FIntVector로 변환 (정수 좌표계이므로 반올림 안전장치)
    FIntVector Up = FIntVector(FMath::RoundToInt(UpVec.X), FMath::RoundToInt(UpVec.Y), FMath::RoundToInt(UpVec.Z));
    FIntVector Right = FIntVector(FMath::RoundToInt(RightVec.X), FMath::RoundToInt(RightVec.Y), FMath::RoundToInt(RightVec.Z));

    // 2. 패턴 생성 (Direction, Up, Right를 조합)
    TArray<TArray<FIntVector>> ResultPatterns;

    ResultPatterns.Add({Direction, Direction + Up, Direction });
    ResultPatterns.Add({Direction, Direction - Up, Direction });
    ResultPatterns.Add({Direction, Direction, Right });
    ResultPatterns.Add({Direction, Direction, Right * -1 });

    return ResultPatterns;
}

FIntVector UChunkGenerator::GetDirectionUsingPattern(TArray<FIntVector> Pattern)
{
    if (Pattern[1].Z == 0) return Pattern[2];
    else return Pattern[1];
}

void UChunkGenerator::GenerateStartLocation()
{
    int p = this->ChunkGrid->ChunkGrid.Num();
    int q = this->ChunkGrid->ChunkGrid[0].Num();
    int r = this->ChunkGrid->ChunkGrid[0][0].Num();

    Visited.Init(0, p * q * r);

    // Start setup
    FIntVector StartDirection = FIntVector(1, 0, 0);
    FIntVector StartIndex = FIntVector(p / 2, q / 2, r / 2);

    // Initial Chunks
    Visited[GetVisitedIndex(StartIndex)] = 1;
    ChunkIndexContainer.Push(StartIndex);
    
    StartIndex += StartDirection;
    Visited[GetVisitedIndex(StartIndex)] = 2;
    ChunkIndexContainer.Push(StartIndex);
    this->DirectionContainer.Add(StartDirection);
}

void UChunkGenerator::DeleteCurrentChunk()
{
    // Remove exactly 3 chunks
    for (int32 i = 0; i < 3; i++)
    {
        FIntVector RemovePos = ChunkIndexContainer.Pop();
        Visited[GetVisitedIndex(RemovePos)] = 0;
        this->ChunkGrid->InsertChunk(RemovePos, EChunkProperty::Empty);
    }

    DirectionContainer.Pop();
}

void UChunkGenerator::GenerateNextChunk(bool& bBlocked)
{
    // Generate a Pattern.
    FIntVector LastDirection = this->DirectionContainer.Last();
    LastDirection.Z = 0;
    TArray<TArray<FIntVector>> Patterns = GetPatternsUsingDirection(LastDirection);

    // Fisher-Yates Shuffle
    for (int32 i = Patterns.Num() - 1; i > 0; i--)
    {
        int32 j = FMath::RandRange(0, i);
        Patterns.Swap(i, j);
    }

    // Find a safe pattern.
    int32 SafePatternIdx = -1;
    bool bInsideChunkGrid = false;
    bool bSafeToPlace = false;
    for (int32 i = 0; i < Patterns.Num(); i++)
    {
        // Simulate the pattern to validate it.
        FIntVector CheckPos = ChunkIndexContainer.Last();
        int32 TempStep = ChunkIndexContainer.Num() + 1;
        TArray<FIntVector> Pattern = Patterns[i];
        for (const FIntVector& Step : Pattern)
        {
            CheckPos += Step;
            bInsideChunkGrid = this->ChunkGrid->IsInsideChunkGrid(CheckPos);
            bSafeToPlace = IsSafeToPlace(CheckPos, TempStep++, NeighborRadius);
            if (bInsideChunkGrid && bSafeToPlace) 
            {
                SafePatternIdx = i;
                break;
            }
        }
    }

    // Apply Pattern to generate next chunk.
    if (SafePatternIdx > -1)
    {
        bBlocked = false;

        FIntVector CurrentPos = ChunkIndexContainer.Last();
        TArray<FIntVector> Pattern = Patterns[SafePatternIdx];
        for (const FIntVector& Step : Pattern)
        {
            CurrentPos += Step;
            Visited[GetVisitedIndex(CurrentPos)] = ChunkIndexContainer.Num() + 1;
            ChunkIndexContainer.Push(CurrentPos);
        }
        this->DirectionContainer.Add(GetDirectionUsingPattern(Pattern));
    }
    // Nowhere to go...
    else
    {
        bBlocked = true;
    }
}

void UChunkGenerator::GenerateCreepWayChunk(int32 ChunkCount)
{
    GenerateStartLocation();

    // --- NEW LOGIC: Pattern Stack ---
    // This stores a list of available patterns for EACH step.
    TArray<TArray<TArray<FIntVector>>> PatternStack;

    int InfCount = 0;
    int MaxCount = 5000000;

    while (ChunkCount > 0 && InfCount++ < MaxCount)
    {
        // 1. If we don't have patterns for the current step, generate them
        if (PatternStack.Num() < DirectionContainer.Num())
        {
            FIntVector LastDirection = this->DirectionContainer.Last();
            LastDirection.Z = 0;
            TArray<TArray<FIntVector>> NewPatterns = GetPatternsUsingDirection(LastDirection);

            // Fisher-Yates Shuffle
            for (int32 i = NewPatterns.Num() - 1; i > 0; i--)
            {
                int32 j = FMath::RandRange(0, i);
                NewPatterns.Swap(i, j);
            }
            PatternStack.Push(NewPatterns);
        }

        // 2. Try the next available pattern at the current level
        bool FoundValidPattern = false;
        TArray<TArray<FIntVector>>& CurrentLevelPatterns = PatternStack.Last();

        while (CurrentLevelPatterns.Num() > 0)
        {
            TArray<FIntVector> Pattern = CurrentLevelPatterns.Pop();
            
            // Check for overlaps after applying the pattern.
            bool GoodPattern = true;
            FIntVector CheckPos = ChunkIndexContainer.Last();
            int32 TempStep = ChunkIndexContainer.Num() + 1;
            for (const FIntVector& Step : Pattern)
            {
                CheckPos += Step;
                if (!this->ChunkGrid->IsInsideChunkGrid(CheckPos)) 
                {
                    GoodPattern = false;
                    break;
                }
                if (!IsSafeToPlace(CheckPos, TempStep++, NeighborRadius))
                {
                    GoodPattern = false;
                    break;
                }
            }

            // Apply Pattern
            if (GoodPattern)
            {
                FIntVector CurrentPos = ChunkIndexContainer.Last();
                for (const FIntVector& Step : Pattern)
                {
                    CurrentPos += Step;
                    Visited[GetVisitedIndex(CurrentPos)] = ChunkIndexContainer.Num() + 1;
                    ChunkIndexContainer.Push(CurrentPos);
                    this->ChunkGrid->InsertChunk(CurrentPos, EChunkProperty::CreepWay);
                }
                this->DirectionContainer.Add(GetDirectionUsingPattern(Pattern));
                
                ChunkCount--;
                FoundValidPattern = true;
                break;
            }
        }

        // 3. Backtracking: If no patterns worked at this level
        if (!FoundValidPattern)
        {
            if (DirectionContainer.Num() <= 1) break; // Cannot backtrack further

            // Remove the 3 chunks added by the previous pattern
            for (int i = 0; i < 3; i++)
            {
                FIntVector RemovePos = ChunkIndexContainer.Pop();
                Visited[GetVisitedIndex(RemovePos)] = 0;
            }

            this->DirectionContainer.Pop();
            PatternStack.Pop(); // Remove the exhausted pattern list
            ChunkCount++;
        }
    }

    // Finalize: Insert into actual grid
    for (const FIntVector& Pos : ChunkIndexContainer)
    {
        this->ChunkGrid->InsertChunk(Pos, EChunkProperty::CreepWay);
    }

    if (InfCount >= MaxCount) UE_LOG(LogTemp, Warning, TEXT("Generation Timed Out (MaxCount reached)"));
}

bool UChunkGenerator::IsSafeToPlace(const FIntVector& TargetPos, int32 CurrentStep, int32 Radius)
{
    // 1. 이미 자리가 차 있으면 당연히 안됨
    if (Visited[GetVisitedIndex(TargetPos)] != 0) return false;

    // 2. 주변(Radius) 탐색
    for (int i = -Radius; i <= Radius; i++)
    {
        for (int j = -Radius; j <= Radius; j++)
        {
            for (int k = -Radius; k <= Radius; k++)
            {
                // 검사할 이웃 좌표
                FIntVector NeighborPos = TargetPos + FIntVector(i, j, k);

                // 그리드 밖이면 무시
                if (!this->ChunkGrid->IsInsideChunkGrid(NeighborPos)) continue;

                int32 NeighborStep = Visited[GetVisitedIndex(NeighborPos)];

                // 빈 공간이면 안전
                if (NeighborStep == 0) continue;

                // [가장 중요한 로직]
                // 무언가 있는데, 그게 "나와 아주 가까운 과거(내 꼬리)"라면 괜찮다.
                // 하지만 "먼 과거(다른 경로)"라면 너무 가깝게 붙은 것이다 -> 충돌!
                
                // 예: Radius가 2라면, 내 바로 전(1), 전전(2) 스텝까지는 반경 안에 들어와도 됨.
                // 하지만 3스텝 이상 차이나는 녀석이 반경 안에 있다면? 그건 꼬인 거임.
                
                // (조금 여유 있게 Radius + 3 정도의 차이는 허용)
                if (FMath::Abs(CurrentStep - NeighborStep) > Radius + 3)
                {
                    return false; // 지금 이웃은 이전에 생성된 청크임!
                }
            }
        }
    }

    return true; // 주변이 깨끗함 (혹은 내 꼬리밖에 없음)
}
int32 UChunkGenerator::GetVisitedIndex(const FIntVector& VisitedIndex)
{
    int p = this->ChunkGrid->ChunkGrid.Num();
    int q = this->ChunkGrid->ChunkGrid[0].Num();
    int r = this->ChunkGrid->ChunkGrid[0][0].Num();
    return VisitedIndex.X + (VisitedIndex.Y * p) + (VisitedIndex.Z * p * q);
}

