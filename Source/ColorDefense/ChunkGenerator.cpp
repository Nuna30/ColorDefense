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
    // ResultPatterns.Add({Direction, Direction - Up, Direction });
    ResultPatterns.Add({Direction, Direction, Right });
    // ResultPatterns.Add({Direction, Direction, Right * -1 });

    return ResultPatterns;
}

FIntVector UChunkGenerator::GetDirectionUsingPattern(TArray<FIntVector> Pattern)
{
    if (Pattern[1].Z == 0) return Pattern[2];
    else return Pattern[1];
}

void UChunkGenerator::GenerateCreepWayChunk(int32 ChunkCount, int32 NeighborRadius)
{
    // 청크 그리드의 크기 가져오기
    int p = this->ChunkGrid->ChunkGrid.Num();
    int q = this->ChunkGrid->ChunkGrid[0].Num();
    int r = this->ChunkGrid->ChunkGrid[0][0].Num();

    // TArray는 3차원 생성 시 2차원까지는 포인터기 때문에 연속된 데이터가 아님 => 특정 원소 접근 시 캐시 미스 발생률이 높음
    // 1차원으로 생성해서 데이터가 연속되도록 구현
    TArray<int32> Visited;
    Visited.Init(0, p * q * r);

    // 경로 생성 시작 방향은 항상 (1, 0, 0)
    // 경로 찾기 전 방향 컨테이너 셋업
    FIntVector StartDirection = FIntVector(1, 0, 0);
    this->DirectionContainer.Add(StartDirection);

    // 경로 시작은 항상 중앙에서부터
    FIntVector StartIndex = FIntVector(p / 2, q / 2, r / 2);

    // 생성될 Chunk들의 Index를 모아두는 곳
    TArray<FIntVector> ChunkIndexContainer;

    // 일단 두 개 박고 시작
    Visited[GetVisitedIndex(StartIndex)] = 1;
    this->ChunkGrid->InsertChunk(StartIndex, EChunkProperty::CreepWay);
    ChunkIndexContainer.Push(StartIndex);

    StartIndex += StartDirection;

    Visited[GetVisitedIndex(StartIndex)] = 2;
    this->ChunkGrid->InsertChunk(StartIndex, EChunkProperty::CreepWay);
    ChunkIndexContainer.Push(StartIndex);

    // DFS 시작. MaxCount만큼만 시도 (무한루프 방지)
    int InfCount = 0;
    int MaxCount = 5000000;
    while (InfCount++ < MaxCount)
    {
        // 이전에 사용한 방향으로 패턴 생성 후 랜덤성을 위한 셔플
        FIntVector LastDirection = this->DirectionContainer.Last(); // 이전에 사용한 방향 들고오기
        LastDirection.Z = 0; // 패턴을 생성하려면 높이축을 0으로 만들어야 함
        TArray<TArray<FIntVector>> Patterns = GetPatternsUsingDirection(LastDirection); // 패턴 생성
        for (int32 i = 0; i < Patterns.Num(); i++) // 셔플
        {
            int32 j = FMath::RandRange(0, i);		
            Patterns.Swap(i, j);
        }

        // 가능한 패턴을 찾고 그 패턴에 따라 청크 컨테이너에 청크 삽입
        bool GoodPath = false; // 가능한 패턴을 찾으면 true가 됨
        for (const TArray<FIntVector>& Pattern : Patterns)
        {
            // 막힘 없이 갈 수 있나 가보기
            bool GoodPattern = true;
            int32 TempCurrentStep = ChunkIndexContainer.Num() + 1; // 패턴 검사할 때만 사용할 스텝 변수가 있어야 함
            FIntVector CheckPos = ChunkIndexContainer.Last(); // 마지막으로 생성한 청크 꺼내서
            for (const FIntVector& Step : Pattern) // 그 청크를 시작으로 패턴을 따라 생성 시뮬레이션 돌리기
            {
                CheckPos = CheckPos + Step;
                if (!this->ChunkGrid->IsInsideChunkGrid(CheckPos)) {GoodPattern = false; break;}
                if (!IsSafeToPlace(Visited,CheckPos, TempCurrentStep, NeighborRadius)) {GoodPattern = false; break;} // 주변에 청크가 존재하는지 검사
                TempCurrentStep++; // 생성 시뮬레이션 내에서만 반영되는 스텝 업데이트
            }    
            // 가능한 패턴이면 글로 가기
            if (GoodPattern)
            {
                // 패턴 다시 가보면서 Visited, ChunkGrid, ChunkIndexContainer 업데이트
                FIntVector LastPos = ChunkIndexContainer.Last();
                int32 RealStep = ChunkIndexContainer.Num() + 1;
                for (const FIntVector& Step : Pattern)
                {
                    LastPos += Step;
                    if (!this->ChunkGrid->IsInsideChunkGrid(LastPos + Step)) break;
                    Visited[GetVisitedIndex(LastPos)] = RealStep;
                    RealStep++;
                    ChunkIndexContainer.Push(LastPos);
                }
                // DirectionContainer에 패턴의 방향 담기
                this->DirectionContainer.Add(GetDirectionUsingPattern(Pattern));
                // 가능한 패턴이 있음을 알림
                GoodPath = true;
                // ChunkCount를 만족하면 경로 생성 종료함!
                ChunkCount--;
                if (ChunkCount == 0) 
                {
                    for (const FIntVector& ChunkIndex : ChunkIndexContainer)
                    {
                        this->ChunkGrid->InsertChunk(ChunkIndex, EChunkProperty::CreepWay);
                    }   
                    return;
                }
                break;
            }
        }
        // 백트래킹
        // 만약 어떤 패턴도 불가능했다면 마지막으로 넣었던 ChunkIndex만 Visited로 놓고 나머지는 방문 안 했다고 침
        // 그리고 ChunkIndexContainer에서 3개를 뺌 (항상 3칸 가므로)
        // 이렇게 하면 깔끔하게 DFS를 구현할 수 있음
        if (!GoodPath)
        {
            for (int i = 0; i < 3; i++)
            {
                FIntVector RemovePos = ChunkIndexContainer.Pop();
                Visited[GetVisitedIndex(RemovePos)] = 0; 
            }
            this->DirectionContainer.Pop();
            ChunkCount++;
        }
    }

    if (InfCount >= MaxCount) UE_LOG(LogTemp, Warning, TEXT("InfLoop"));
}

bool UChunkGenerator::IsSafeToPlace(TArray<int32>& Visited, const FIntVector& TargetPos, int32 CurrentStep, int32 Radius)
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
                if (FMath::Abs(CurrentStep - NeighborStep) > Radius + 5)
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

