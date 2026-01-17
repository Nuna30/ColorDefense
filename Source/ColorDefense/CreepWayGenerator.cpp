// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils.h"
#include "CreepWayGenerator.h"
#include "CreepRail.h"

#define DEBUGMODE 1

UCreepWayGenerator::UCreepWayGenerator()
{
}

void UCreepWayGenerator::Initialize
(
	UWorld* InWorld, 
	UBPActorPool* InBPActorPool, 
	UVoxelGrid* InVoxelGrid, 
	TArray<UCreepCheckPointGenerator*>& InCreepCheckPointGenerators, 
	int32 InMaxRailCount, 
	int32 InRailLength, 
	TArray<FIntVector> InDirectionContainer,
	float InVoxelWidth, 
	float InVoxelHeight
)
{
	// Initialize variables using parameters.
    Super::Initialize(InWorld, InBPActorPool, InVoxelGrid, InVoxelWidth, InVoxelHeight);
	this->CreepCheckPointGenerators = InCreepCheckPointGenerators;
	this->DirectionContainer = InDirectionContainer;
	
	// Initialize the others.
	this->CurrentDirection = FIntVector(1, 0, 0);
	FIntVector StartIndex = this->VoxelGrid->GetCenterIndex();
	
	this->CreepRail = NewObject<UCreepRail>(this, UCreepRail::StaticClass());
	this->CreepRail->Initialize(InMaxRailCount, InRailLength, StartIndex, this->CurrentDirection);
}

void UCreepWayGenerator::GenerateCreepWay(int32 GenerationStep)
{
	// 일단 두 칸 가고
	this->CreepRail->InsertCreepWayDataRectangleIntoRailBuffers(this->CurrentDirection, 0, false);
	this->CreepRail->UpdateLastIndexesOfEachRail();
	this->CreepRail->InsertCreepWayDataRectangleIntoRailBuffers(this->CurrentDirection, 0, false);
	this->CreepRail->UpdateLastIndexesOfEachRail();

	// 이후 DirectionContainer를 보고 CreepWay 생성
	// 근데 첫 번째 Direction은 ChunkGenerator에서만 쓰이는 원소였어서 여기서는 무시해야함

	for (int i = 1; i < this->DirectionContainer.Num(); i++)
	{
		this->NextDirection = this->DirectionContainer[i];
		if (this->NextDirection.Z == 0) GoStraightAndTurnLeftOrRightAndGoStraight();
		else GoStraightAndUpOrDownAndGoStraight();
	}

	FlushRailBuffersToMainBuffer();
	SpawnActorWithFlushingMainBuffer(); // 나중에는 이 부분을 크립 삭제 마다 버퍼 하나 비우는 식으로 구현해야 한다.
}

void UCreepWayGenerator::SpawnActorWithFlushingMainBuffer()
{
	for (const TTuple<FIntVector, int32, float, EVoxelProperty>& CreepWayData : this->CreepRail->MainBuffer)
	{
		SetVoxelDataInVoxelGrid(CreepWayData.Get<0>(), CreepWayData.Get<1>(), CreepWayData.Get<2>(), CreepWayData.Get<3>());
		SpawnActorFromVoxel(this->VoxelGrid->GetVoxel(CreepWayData.Get<0>()));
		SpawnInvisibleNeighboringPlaceables(CreepWayData.Get<0>());
	}
}

void UCreepWayGenerator::FlushRailBuffersToMainBuffer()
{
	// 1번 레일부터 차례로 버퍼 속 복셀들을 메인 버퍼로 옮긴다.
	for (int32 i = 0; i < this->CreepRail->RailBuffers.Num(); i++)
	{
		for (TTuple<FIntVector, int32, float, EVoxelProperty>& RailElement : this->CreepRail->RailBuffers[i])
		{
			FVoxel& TargetVoxel = this->VoxelGrid->GetVoxel(RailElement.Get<0>());
			if (TargetVoxel.Property == EVoxelProperty::CreepCheckPoint)
			{
				this->CreepCheckPointGenerators[i]->CreateCreepCheckPointByVoxelIndex(RailElement.Get<0>() + FIntVector(0, 0, 1));
			}
		}
		this->CreepRail->MainBuffer.Append(MoveTemp(this->CreepRail->RailBuffers[i]));
	}
}

void UCreepWayGenerator::GoStraightAndTurnLeftOrRightAndGoStraight()
{
	// 일단 앞으로 직진
	this->CreepRail->InsertCreepWayDataRectangleIntoRailBuffers(this->CurrentDirection, 0, false);
	// 코너 구간을 생성하기 전, 최하단 Rail과 최상단 Rail 중 누가 In/Out인지 설정한다.
	UpdateTopRailIn();
	// 코너 구간 생성
	this->CreepRail->InsertCreepWayDataTriangleIntoRailBuffers(this->bTopRailIn, CurrentDirection, 1);
	SetLastIndexesOfEachRailToCreepCheckPoint();
	this->CreepRail->InsertCreepWayDataTriangleIntoRailBuffers(this->bTopRailIn, NextDirection, 0);
	// 다시 직진
	this->CurrentDirection = this->NextDirection;
	this->CreepRail->InsertCreepWayDataRectangleIntoRailBuffers(this->CurrentDirection, 0, false);
}	

void UCreepWayGenerator::GoStraightAndUpOrDownAndGoStraight()
{
	// 앞으로 직진
	this->CreepRail->InsertCreepWayDataRectangleIntoRailBuffers(this->CurrentDirection, 0, false);
	this->CreepRail->UpdateLastIndexesOfEachRail();
	// 위아래 이동 + 경사면 진입 전과 후에 CreepCheckPoint 심기
	this->CurrentDirection.Z = this->NextDirection.Z;
	SetLastIndexesOfEachRailToCreepCheckPoint();
	this->CreepRail->InsertCreepWayDataRectangleIntoRailBuffers(this->CurrentDirection, 1, true);
	SetLastIndexesOfEachRailToCreepCheckPoint();
	this->CreepRail->UpdateLastIndexesOfEachRail();
	// 위로 향하는 경사면이었으면 다음 평면 블록 설치 시 한칸 더 올려서 설치해야하기 때문에 로직에 LastIndexes들을 한칸 올린다.
	if (this->CurrentDirection.Z == 1)
	{
		for (int i = 0; i < this->CreepRail->LastIndexesOfEachRail.Num(); i++)
			this->CreepRail->LastIndexesOfEachRail[i] += FIntVector(0, 0, 1);
	}
	// 평면화 해주고 다시 직진
	this->CurrentDirection.Z = 0;
	this->CreepRail->InsertCreepWayDataRectangleIntoRailBuffers(this->CurrentDirection, 0, false);
	this->CreepRail->UpdateLastIndexesOfEachRail();
	// 현재 방향과 다음 방향이 같으니 갱신 작업은 안 해줘도 됨
}

void UCreepWayGenerator::SetLastIndexesOfEachRailToCreepCheckPoint()
{
	// 각 레일의 마지막 인덱스들에 해당하는 복셀의 속성을 CreepCheckPoint로 설정
	for (int32 i = 0; i < this->CreepRail->MaxRailCount; i++)
	{
		FIntVector LastVoxelIndex = this->CreepRail->LastIndexesOfEachRail[i];
		this->VoxelGrid->VoxelGrid[LastVoxelIndex.X][LastVoxelIndex.Y][LastVoxelIndex.Z].Property = EVoxelProperty::CreepCheckPoint;
	}
}

void UCreepWayGenerator::UpdateTopRailIn()
{
	int32 c = Utils::CardinalDirections.Find(CurrentDirection);
	int32 n = Utils::CardinalDirections.Find(NextDirection);

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

void UCreepWayGenerator::SpawnInvisibleNeighboringPlaceables(const FIntVector& VoxelIndex)
{
	TArray<FIntVector> Neighbor4 = 
	{
		FIntVector(1, 0, 0), FIntVector(-1, 0, 0), FIntVector(0, 1, 0), FIntVector(0, -1, 0)
	};

	// Set Neighbors to Placeable and spawn as invisible
	for (FIntVector& Neighbor : Neighbor4)
	{
		Neighbor += VoxelIndex;
		if (!VoxelGrid->IsInsideVoxelGrid(Neighbor)) continue; // Handle the boundary value

		FVoxel& TargetVoxel = VoxelGrid->GetVoxel(Neighbor);
		if (TargetVoxel.Property == EVoxelProperty::Empty)
		{
			SetVoxelDataInVoxelGrid(Neighbor, 4, 0, EVoxelProperty::PlayerBlock);
			AActor* Placeable = SpawnActorFromVoxel(TargetVoxel);
			Placeable->SetActorHiddenInGame(true);
			APlayerBlock* PlayerBlock = Cast<APlayerBlock>(Placeable);
			PlayerBlock->Voxel = TargetVoxel;
			PlayerBlock->SetOpacity(0.1);
			PlayerBlock->SetCollisionWithPawn(false);
		}
	}

	// // Placing block above logic (deprecated)
	// FVoxel& TargetVoxel = VoxelGrid->GetVoxel(VoxelIndex);
	// if (TargetVoxel.Property == EVoxelProperty::NormalCreepWay) return; // No PlayerBlock above Creepway
	
	// FIntVector TargetIndex = VoxelIndex + FIntVector(0, 0, 1);
	// if (!VoxelGrid->IsInsideVoxelGrid(TargetIndex)) return; // Handle the boundary
	
	// FVoxel& PlayerBlockVoxel = VoxelGrid->GetVoxel(TargetIndex);
	// SetVoxelDataInVoxelGrid(TargetIndex, 4, EVoxelProperty::PlayerBlock);
	// AActor* Placeable = SpawnActorFromVoxel(PlayerBlockVoxel);
	// Placeable->SetActorHiddenInGame(true);
	// APlayerBlock* PlayerBlock = Cast<APlayerBlock>(Placeable);
	// PlayerBlock->Voxel = PlayerBlockVoxel;
	// PlayerBlock->SetOpacity(0.1);
	// PlayerBlock->SetCollisionWithPawn(false);
}