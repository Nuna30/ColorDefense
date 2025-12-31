// Fill out your copyright notice in the Description page of Project Settings.

#include "ChunkGenerator.h"

#define DEBUGMODE 1

UChunkGenerator::UChunkGenerator()
{
}

void UChunkGenerator::Initialize(UChunkGrid* InChunkGrid)
{
    // The Reason the goat climbs the mountain is its stubbornness.
    this->ChunkGrid = InChunkGrid;
}

TArray<TArray<FIntVector>> UChunkGenerator::GetPatternsUsingDirection(FIntVector Direction)
{
    // 1. 기준이 되는 '위쪽(Up)'과 '오른쪽(Right)' 벡터 구하기
    FVector FwdVec = FVector(Direction.X, Direction.Y, Direction.Z);
    FVector UpVec;
    FVector RightVec;

    // 만약 수직 이동 중(위/아래)이라면 -> 기준 Up을 월드 X축으로 설정 (임의 설정)
    if (Direction.Z != 0) 
    {
        UpVec = FVector(1, 0, 0); 
    }
    // 수평 이동 중이라면 -> 기준 Up은 월드 Z축 (0, 0, 1)
    else 
    {
        UpVec = FVector(0, 0, 1);
    }

    // 외적(Cross Product)을 이용해 '오른쪽' 벡터 자동 계산
    // Right = Up x Direction (순서는 좌표계에 따라 다를 수 있으니 테스트 필요)
    RightVec = FVector::CrossProduct(UpVec, FwdVec);

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

void UChunkGenerator::GenerateCreepWayChunk(int ChunkCount)
{
    // 청크 그리드의 크기 가져오기
    int p = this->ChunkGrid->ChunkGrid.Num();
    int q = this->ChunkGrid->ChunkGrid[0].Num();
    int r = this->ChunkGrid->ChunkGrid[0][0].Num();

    // TArray는 3차원 생성 시 2차원까지는 포인터기 때문에 연속된 데이터가 아님 => 특정 원소 접근 시 캐시 미스 발생률이 높음
    // 1차원으로 생성해서 데이터가 연속되도록 구현
    TArray<bool> Visited;
    Visited.Init(false, p * q * r);

    // Visited 3차원 인덱스->1차원 인덱스 계산 람다 함수
    auto GetIndex = [p, q](int32 X, int32 Y, int32 Z) -> int32 {
        return X + (Y * p) + (Z * p * q);
    };

    // 경로 생성 시작 방향은 항상 (1, 0, 0)
    // 경로 찾기 전 방향 컨테이너 셋업
    FIntVector StartDirection = FIntVector(1, 0, 0);
    this->DirectionContainer.Add(StartDirection);

    // 경로 시작은 항상 중앙에서부터
    FIntVector StartIndex = FIntVector(p / 2, q / 2, r / 2);

    // 생성될 Chunk들의 Index를 모아두는 곳
    TArray<FIntVector> ChunkIndexContainer;

    // 일단 두 개 박고 시작
    Visited[GetIndex(StartIndex.X, StartIndex.Y, StartIndex.Z)] = true;
    this->ChunkGrid->InsertChunk(StartIndex, EChunkProperty::CreepWay);
    ChunkIndexContainer.Push(StartIndex);
    StartIndex += StartDirection;
    Visited[GetIndex(StartIndex.X, StartIndex.Y, StartIndex.Z)] = true;
    this->ChunkGrid->InsertChunk(StartIndex, EChunkProperty::CreepWay);
    ChunkIndexContainer.Push(StartIndex);

    while (true)
    {
        // 패턴 셔플
        FIntVector LastDirection = DirectionContainer.Last();
        LastDirection.Z = 0;
        TArray<TArray<FIntVector>> Patterns = GetPatternsUsingDirection(LastDirection);
        for (int32 i = 0; i < Patterns.Num(); i++)
        {
            int32 j = FMath::RandRange(0, i);		
            Patterns.Swap(i, j);
        }

        // 가능한 패턴 찾기
        bool GoodPath = false;
        for (const TArray<FIntVector>& Pattern : Patterns)
        {
            bool GoodPattern = true;
            // 막힘 없이 갈 수 있나 가보기
            FIntVector TempPos = ChunkIndexContainer.Last();
            for (const FIntVector& Step : Pattern)
            {
                TempPos += Step;
                if (TempPos.X < 0 || TempPos.X >= p) {GoodPattern = false; break;}
                if (TempPos.Y < 0 || TempPos.Y >= q) {GoodPattern = false; break;}
                if (TempPos.Z < 0 || TempPos.Z >= r) {GoodPattern = false; break;}
                if (Visited[GetIndex(TempPos.X, TempPos.Y, TempPos.Z)]) {GoodPattern = false; break;}
            }    
            // 가능한 패턴이면 글로 가기
            if (GoodPattern)
            {
                // 패턴 다시 가보면서 Visited, ChunkGrid, ChunkIndexContainer 업데이트
                FIntVector Current = ChunkIndexContainer.Last();
                for (const FIntVector& Step : Pattern)
                {
                    Current += Step;
                    Visited[GetIndex(Current.X, Current.Y, Current.Z)] = true;
                    this->ChunkGrid->InsertChunk(Current, EChunkProperty::CreepWay);
                    ChunkIndexContainer.Push(Current);
                }
                // DirectionContainer에 패턴의 방향 담기
                DirectionContainer.Add(GetDirectionUsingPattern(Pattern));
                // 가능한 패턴이 있음을 알림
                GoodPath = true;
                // ChunkCount를 만족하면 경로 생성 종료함!
                ChunkCount--;
                if (ChunkCount == 0) return;
                break;
            }
        }
        // 만약 어떤 패턴도 불가능했다면 마지막으로 넣었던 ChunkIndex만 Visited로 놓고 나머지는 방문 안 했다고 침
        // 그리고 ChunkIndexContainer에서 3개를 뺌 (항상 3칸 가므로)
        // 이렇게 하면 깔끔하게 DFS를 구현할 수 있음
        if (!GoodPath)
        {
            ChunkIndexContainer.Pop();
            ChunkIndexContainer.Pop();
            Visited[GetIndex(ChunkIndexContainer.Last().X, ChunkIndexContainer.Last().Y, ChunkIndexContainer.Last().Z)] = false;
            ChunkIndexContainer.Pop();
            Visited[GetIndex(ChunkIndexContainer.Last().X, ChunkIndexContainer.Last().Y, ChunkIndexContainer.Last().Z)] = false;
            DirectionContainer.Pop();
        }
    }
}