// Fill out your copyright notice in the Description page of Project Settings.


#include "CreepWayGenerator.h"

#define DEBUGMODE 1

UCreepWayGenerator::UCreepWayGenerator()
{
}

void UCreepWayGenerator::Initialize(UWorld* InWorld, UBPActorPool* InBPActorPool, UVoxelGrid* InVoxelGrid, UChunkGrid* InChunkGrid, TArray<UCreepCheckPointGenerator*>& InCreepCheckPointGenerators, int32 InMaxRailCount, int32 InRailLength)
{
    Super::Initialize(InWorld, InBPActorPool, InVoxelGrid);
	// 최대 레일 개수
	this->MaxRailCount = InMaxRailCount;
	// 레일 길이
	this->RailLength = InRailLength;
	// ODirectionArray은 사방향 벡터를 저장한다.
	this->ODirectionArray = {
		FIntVector(1, 0, 0), FIntVector(-1, 0, 0), FIntVector(0, 1, 0), FIntVector(0, -1, 0)
	};
	// RailBuffers는 각 레일에 해당하는 버퍼들을 저장한다.
	this->RailBuffers.SetNum(MaxRailCount);
	// CurrentDirection은 현재 경로 진행 방향을 저장한다.
	this->CurrentDirection = FIntVector(1, 0, 0);
	// LastIndexesOfEachRail은 각 레일의 마지막 CreepWayBlock의 인덱스를 저장한다.
	FIntVector StartVoxelGridIndex = FIntVector(this->VoxelGrid->VoxelGridSize.X / 2, this->VoxelGrid->VoxelGridSize.Y / 2, this->VoxelGrid->VoxelGridSize.Z / 2);
	for (int32 i = 0; i < this->MaxRailCount; i++)
	{
		this->LastIndexesOfEachRail.Add(StartVoxelGridIndex);
		StartVoxelGridIndex = StartVoxelGridIndex + GetPerpendicularDirection(this->CurrentDirection);
	}
	// CreepCheckPointGenerators를 연결한다.
	this->CreepCheckPointGenerators = InCreepCheckPointGenerators;
	// 청크 그리드를 CreepWayGenerator와 Coupling하고 그리드의 중앙을 가르키는 Index를 얻는다. 생성은 CreepWay와 같이 진행한다.
	this->ChunkGrid = InChunkGrid;
	FIntVector StartChunkGridIndex = FIntVector(this->ChunkGrid->ChunkGridSize.X / 2, this->ChunkGrid->ChunkGridSize.Y / 2, this->ChunkGrid->ChunkGridSize.Z / 2);
	this->CurrentChunkIndex = StartChunkGridIndex;
	this->ChunkGrid->InsertChunk(StartChunkGridIndex, EChunkProperty::CreepWay);
}

void UCreepWayGenerator::GenerateCreepWay(int32 GenerationStep)
{
	// 맨 처음 영역 생성
	// InitializeCreepWay();

	for (int32 i = 0; i < GenerationStep; i++)
	{
		DecideNextDirection();
		if (this->CurrentDirection == this->NextDirection) GoStraightAndUpOrDownAndGoStraight();
		else GoStraightAndTurnLeftOrRightAndGoStraight();
		this->CurrentDirection = this->NextDirection;
	}

	FlushRailBuffersToMainBuffer();
	SpawnActorWithFlushingMainBuffer(); // 나중에는 이 부분을 크립 삭제 마다 버퍼 하나 비우는 식으로 구현해야 한다.
	SpawnAllUsingChunkGrid();
}

void UCreepWayGenerator::SpawnActorWithFlushingMainBuffer()
{
	for (const FIntVector& VoxelIndex : MainBuffer)
	{
		FIntVector TargetVoxelIndex = VoxelIndex;
		FVoxel TargetVoxel = this->VoxelGrid->GetVoxel(VoxelIndex);
		SpawnActorFromVoxel(TargetVoxel);
	}
}

void UCreepWayGenerator::FlushRailBuffersToMainBuffer()
{
	for (int32 i = 0; i < this->RailBuffers.Num(); i++)
	{
		for (FIntVector& VoxelIndex : RailBuffers[i])
		{
			FVoxel& TargetVoxel = this->VoxelGrid->GetVoxel(VoxelIndex);
			if (TargetVoxel.Property == EVoxelProperty::CreepCheckPoint)
			{
				this->CreepCheckPointGenerators[i]->CreateCreepCheckPointByVoxelIndex(VoxelIndex + FIntVector(0, 0, 1));
			}
		}
		MainBuffer.Append(MoveTemp(RailBuffers[i]));
	}
}

void UCreepWayGenerator::InitializeCreepWay()
{
	if (this->CurrentDirection == this->NextDirection) GoStraightAndUpOrDownAndGoStraight();
	else GoStraightAndTurnLeftOrRightAndGoStraight();

	// 나머지 세팅 (CreepStart, CreepEnd 설치, ...)
}

void UCreepWayGenerator::GoStraightAndTurnLeftOrRightAndGoStraight()
{
	// 일단 앞으로 직진
	LoadVoxelIndexRectangleIntoRailBuffers(0, false);
	// this->ChunkGrid->InsertChunk(this->CurrentChunkIndex, EChunkProperty::CreepWay);
	// this->CurrentChunkIndex += this->CurrentDirection;
	// 코너 구간을 생성하기 전, 최하단 Rail과 최상단 Rail 중 누가 In/Out인지 설정한다.
	UpdateTopRailIn();
	// 코너 구간 생성
	LoadVoxelIndexTriangleIntoRailBuffers(CurrentDirection);
	SetLastIndexesOfEachRailToCreepCheckPoint();
	LoadVoxelIndexTriangleIntoRailBuffers(NextDirection);
	// this->ChunkGrid->InsertChunk(this->CurrentChunkIndex, EChunkProperty::CreepWay);
	// this->CurrentChunkIndex += this->NextDirection;
	// 다시 직진
	this->CurrentDirection = this->NextDirection;
	LoadVoxelIndexRectangleIntoRailBuffers(0, false);
	// this->ChunkGrid->InsertChunk(this->CurrentChunkIndex, EChunkProperty::CreepWay);
	// this->CurrentChunkIndex += this->CurrentDirection;
}	

void UCreepWayGenerator::GoStraightAndUpOrDownAndGoStraight()
{
	// 앞으로 직진
	LoadVoxelIndexRectangleIntoRailBuffers(0, false);
	UpdateLastIndexesOfEachRail();
	// this->ChunkGrid->InsertChunk(this->CurrentChunkIndex, EChunkProperty::CreepWay);
	// 위아래 정하기
	TArray<int32> UpOrDown = {-1, 1};
	this->CurrentDirection.Z = UpOrDown[FMath::RandRange(0, 1)];
	// 위아래 직진
	LoadVoxelIndexRectangleIntoRailBuffers(1, true);
	UpdateLastIndexesOfEachRail();
	// this->CurrentChunkIndex += this->CurrentDirection; // 위아래 방향 포함, 높이 결정된 후에 이동
	// this->ChunkGrid->InsertChunk(this->CurrentChunkIndex, EChunkProperty::CreepWay);
	// this->CurrentChunkIndex += this->CurrentDirection; // 기존 방식대로 이동
	// 평면화 해주고 다시 직진
	this->CurrentDirection.Z = 0;
	LoadVoxelIndexRectangleIntoRailBuffers(0, false);
	UpdateLastIndexesOfEachRail();
	// this->ChunkGrid->InsertChunk(this->CurrentChunkIndex, EChunkProperty::CreepWay);
	// this->CurrentChunkIndex += this->CurrentDirection;
	// 현재 방향과 다음 방향이 같으니 갱신 작업은 안 해줘도 됨
}

void UCreepWayGenerator::SetLastIndexesOfEachRailToCreepCheckPoint()
{
	// 각 레일의 마지막 인덱스들에 해당하는 복셀의 속성을 CreepCheckPoint로 설정
	for (int32 i = 0; i < this->MaxRailCount; i++)
	{
		FIntVector LastVoxelIndex = this->LastIndexesOfEachRail[i];
		this->VoxelGrid->VoxelGrid[LastVoxelIndex.X][LastVoxelIndex.Y][LastVoxelIndex.Z].Property = EVoxelProperty::CreepCheckPoint;
	}
}

void UCreepWayGenerator::LoadVoxelIndexTriangleIntoRailBuffers(const FIntVector& Direction)
{
	// bTopRailIn 변수를 고려해서 CurrentDirection 방향으로 삼각형 영역의 VoxelIndex를 버퍼에 로딩
	for (int32 i = 0; i < MaxRailCount; i++)
	{
		FIntVector VoxelIndexForRailFromBottom = this->LastIndexesOfEachRail[i];
		TArray<FIntVector>& RailBufferFromBottom = this->RailBuffers[i];
		FIntVector VoxelIndexForRailFromTop = this->LastIndexesOfEachRail[MaxRailCount - i - 1];
		TArray<FIntVector>& RailBufferFromTop = this->RailBuffers[MaxRailCount - i - 1];
		for (int32 j = 0; j < i; j++)
		{
			// TopRail이 In 상태이면 TopRailBuffer부터 처리해줘야한다.
			if (bTopRailIn)
			{
				// 다음 칸으로 이동 (이 코드가 맨 앞으로 와야 LastIndex 앞에 설치함)
				VoxelIndexForRailFromTop = VoxelIndexForRailFromTop + Direction;
				// 인덱스가 청크 안에 존재하면 RailBuffer에 넣고 VoxelGrid에 VoxelData를 생성한다.
				if (!this->VoxelGrid->IsInsideVoxelGrid(VoxelIndexForRailFromTop))
					this->VoxelGrid->ExpandVoxelGrid(VoxelIndexForRailFromTop);
				RailBufferFromTop.Add(VoxelIndexForRailFromTop);
				this->LastIndexesOfEachRail[MaxRailCount - i - 1] = VoxelIndexForRailFromTop;
				this->SetVoxelDataInVoxelGrid(VoxelIndexForRailFromTop, 0, EVoxelProperty::NormalCreepWay);
			}
			else
			{
				// 다음 칸으로 이동 (이 코드가 맨 앞으로 와야 LastIndex 앞에 설치함)
				VoxelIndexForRailFromBottom = VoxelIndexForRailFromBottom + Direction;
				// 인덱스가 청크 안에 존재하면 RailBuffer에 넣고 VoxelGrid에 VoxelData를 생성한다.
				if (!this->VoxelGrid->IsInsideVoxelGrid(VoxelIndexForRailFromBottom))
					this->VoxelGrid->ExpandVoxelGrid(VoxelIndexForRailFromBottom);
				this->LastIndexesOfEachRail[i] = VoxelIndexForRailFromBottom;
				RailBufferFromBottom.Add(VoxelIndexForRailFromBottom);
				this->SetVoxelDataInVoxelGrid(VoxelIndexForRailFromBottom, 0, EVoxelProperty::NormalCreepWay);
			}
		}
	}
}

void UCreepWayGenerator::LoadVoxelIndexRectangleIntoRailBuffers(int32 BPActorPoolIndex, bool bRotate)
{
	// 오르막길이면 높이 하나를 낮춰줘야 한다.
	FIntVector DownOffset = FIntVector(0, 0, 0);
	if (this->CurrentDirection.Z == 1) DownOffset.Z = -1;
	// CurrentDirection 방향으로 직사각형 영역의 VoxelIndex를 버퍼에 로딩
	for (int32 i = 0; i < this->MaxRailCount; i++)
	{
		FIntVector VoxelIndexForRail = this->LastIndexesOfEachRail[i] + DownOffset;
		TArray<FIntVector>& RailBuffer = this->RailBuffers[i];					
		for (int32 j = 0; j < this->RailLength; j++)
		{
			// 다음 칸으로 이동 (이 코드가 맨 앞으로 와야 LastIndex 앞에 설치함)
			VoxelIndexForRail = VoxelIndexForRail + this->CurrentDirection;

			// 인덱스가 청크 안에 존재하면 RailBuffer에 넣고 VoxelGrid에 VoxelData를 생성한다.
			if (this->VoxelGrid->IsInsideVoxelGrid(VoxelIndexForRail))
			{
				RailBuffer.Add(VoxelIndexForRail);
				this->LastIndexesOfEachRail[i] = VoxelIndexForRail;
				SetVoxelDataInVoxelGrid(VoxelIndexForRail, BPActorPoolIndex, EVoxelProperty::NormalCreepWay);
				// Up/Down 유무와 방향에 맞게 SlopeCreepWayBlock을 회전시킨다.
				if (bRotate) RotateSlopeCreepWayBlock(VoxelIndexForRail);
			}
			else if (DEBUGMODE)
			{
				FString DebugMessage = FString::Printf(TEXT("UCreepWayGenerator::LoadVoxelIndexRectangleIntoRailBuffers {VoxelIndexForRail is not inside VoxelGrid!}"));
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, DebugMessage);
				UE_LOG(LogTemp, Error, TEXT("%s"), *DebugMessage);					
				return;
			}
		}
	}
	// 오르막길이면 마지막에 한 칸 높여서 평면 블록을 설치해줘야 한다.
	if (this->CurrentDirection.Z == 1)
	{
		for (int32 i = 0; i < this->MaxRailCount; i++)
		{
			FIntVector VoxelIndexForRail = this->LastIndexesOfEachRail[i] + this->CurrentDirection;
			this->RailBuffers[i].Add(VoxelIndexForRail);
			this->LastIndexesOfEachRail[i] = VoxelIndexForRail;
			// Slope를 쓰다가 이 부분만 평면을 사용한다. 그래서 그냥 하드코딩 했다. 위험 부분.
			SetVoxelDataInVoxelGrid(VoxelIndexForRail, 0, EVoxelProperty::NormalCreepWay);
		}
	}
}

void UCreepWayGenerator::UpdateLastIndexesOfEachRail()
{
	// LastIndexesOfEachRail 갱신
	for (int32 i = 0; i < this->MaxRailCount; i++)
	{
		this->LastIndexesOfEachRail[i] = RailBuffers[i].Last();
	}
}

FIntVector UCreepWayGenerator::GetPerpendicularDirection(const FIntVector& Direction)
{
	if (Direction.X == 1) return FIntVector(0, 1, Direction.Z);
	else if (Direction.Y == 1) return FIntVector(1, 0, Direction.X);
	else return FIntVector::ZeroValue;
}

void UCreepWayGenerator::DecideNextDirection()
{
	// 방향배열 원본 복사
	TArray<FIntVector> DirectionArray = ODirectionArray;
	// 방향벡터 평면화
	NextDirection.Z = 0;
	// 반대 방향 삭제
	DirectionArray.Remove(CurrentDirection * -1);
	// 방향배열에 무작위성 추가 (Fisher-Yates 셔플)
	for (int32 i = 0; i < DirectionArray.Num(); i++)
	{
		int32 j = FMath::RandRange(0, i);		
		DirectionArray.Swap(i, j);
	}

	// 경로 생성해도 되는 방향 선택
	for (int32 i = 0; i < DirectionArray.Num(); i++)
	{
		// CreepWay가 비어있는 방향인지 확인
		FIntVector MaybeNextDirection = DirectionArray[i];
		FIntVector CenterRailLastIndex = this->LastIndexesOfEachRail[this->MaxRailCount / 2];
		// 체크하기 전 우선 현재 CreepWay를 벗어나고
		CenterRailLastIndex += FIntVector(
			MaybeNextDirection.X * this->RailLength, MaybeNextDirection.Y * this->RailLength, MaybeNextDirection.Z * this->RailLength
		);
		// 범위 내를 전부 체크
		bool bKeepGoing = true;
		for (int32 j = this->RailLength / 2 + 1; j < this->RailLength * 2; j++)
		{
			CenterRailLastIndex = CenterRailLastIndex + MaybeNextDirection;
			if (!this->VoxelGrid->IsEmptyIndex(CenterRailLastIndex)) // 이거 나중에 사용자 블록은 제외시켜야 할 듯?
			{
				bKeepGoing = false;
				break;
			}
		}
		// 비어있으면 NextDirection 결정, 함수 종료
		if (bKeepGoing) 
		{
			this->NextDirection = MaybeNextDirection;
			return;
		}
	}

	// 경로 못 찾을 시 
	// 미구현
}

void UCreepWayGenerator::RotateSlopeCreepWayBlock(const FIntVector& VoxelIndex)
{
	if (this->CurrentDirection == FIntVector(1, 0, 1)) this->VoxelGrid->SetRotation(VoxelIndex, 270);
	else if (this->CurrentDirection == FIntVector(-1, 0, 1)) this->VoxelGrid->SetRotation(VoxelIndex, 90);
	else if (this->CurrentDirection == FIntVector(0, 1, 1)) this->VoxelGrid->SetRotation(VoxelIndex, 0);
	else if (this->CurrentDirection == FIntVector(0, -1, 1)) this->VoxelGrid->SetRotation(VoxelIndex, 180);
	else if (this->CurrentDirection == FIntVector(1, 0, -1)) this->VoxelGrid->SetRotation(VoxelIndex, 90);
	else if (this->CurrentDirection == FIntVector(-1, 0, -1)) this->VoxelGrid->SetRotation(VoxelIndex, 270);
	else if (this->CurrentDirection == FIntVector(0, 1, -1)) this->VoxelGrid->SetRotation(VoxelIndex, 180);
	else if (this->CurrentDirection == FIntVector(0, -1, -1)) this->VoxelGrid->SetRotation(VoxelIndex, 0);
}

void UCreepWayGenerator::UpdateTopRailIn()
{
	int32 c = this->ODirectionArray.Find(CurrentDirection);
	int32 n = this->ODirectionArray.Find(NextDirection);

	if (c == 0)
	{
		if (n == 2) this->bTopRailIn = true;
		else this->bTopRailIn = false;
	}
	else if (c == 1)
	{
		if (n == 2) this->bTopRailIn = false;
		else this->bTopRailIn = true;
	}
	else if (c == 2)
	{
		if (n == 0) this->bTopRailIn = false;
		else this->bTopRailIn = true;
	}
	else
	{
		if (n == 0) this->bTopRailIn = true;
		else this->bTopRailIn = false;
	}
}

void UCreepWayGenerator::PrintLastIndexes()
{
	for (int32 i = 0; i < this->LastIndexesOfEachRail.Num(); i++)
	{
		print(FString::Printf(TEXT("LastIndexesOfEachRail[%d] : %d %d %d"), i, LastIndexesOfEachRail[i].X, LastIndexesOfEachRail[i].Y, LastIndexesOfEachRail[i].Z));		
	}
}

void UCreepWayGenerator::PrintRailBuffers()
{
	for (int32 i = 0; i < RailBuffers.Num(); i++)
	{
		for (int32 j = 0; j < RailBuffers[i].Num(); j++)
		{
			print(FString::Printf(TEXT("RailBuffers[%d][%d] : %d %d %d"), i, j, RailBuffers[i][j].X, RailBuffers[i][j].Y, RailBuffers[i][j].Z));
		}
	}
}

void UCreepWayGenerator::PrintDirections()
{
	print(FString::Printf(TEXT("Current Direction, NextDirection : (%d %d %d), (%d %d %d)"),
	 CurrentDirection.X, CurrentDirection.Y, CurrentDirection.Z, NextDirection.X, NextDirection.Y, NextDirection.Z));
}

void UCreepWayGenerator::print(FString DebugMessage)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, DebugMessage);
	UE_LOG(LogTemp, Error, TEXT("%s"), *DebugMessage);		
}

void UCreepWayGenerator::SpawnAllUsingChunkGrid()
{
	for (const TArray<TArray<FChunk>>& C2 : this->ChunkGrid->ChunkGrid)
	{
		for (const TArray<FChunk>& C1 : C2)
		{
			for (const FChunk& C : C1)
			{
				if (C.Property == EChunkProperty::CreepWay)
				{
					AActor* NewActor = World -> SpawnActor<AActor> (
						this->BPActorPool->Pool[0],
						FVector(250 * C.Index.X, 250 * C.Index.Y, 125 * C.Index.Z),
						FRotator(0, 0, 0)
					);
				}
			}
		}
	}
}