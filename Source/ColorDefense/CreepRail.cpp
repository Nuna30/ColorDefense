// Fill out your copyright notice in the Description page of Project Settings.

#include "CreepRail.h"
#include "Utils.h"

UCreepRail::UCreepRail()
{

}

void UCreepRail::Initialize(int32 InMaxRailCount, int32 InRailLength, FIntVector StartIndex, FIntVector CurrentDirection)
{
	this->MaxRailCount = InMaxRailCount;
    this->RailLength = InRailLength;
    this->RailBuffers.SetNum(InMaxRailCount);

	for (int32 i = 0; i < InMaxRailCount; i++)
	{
		this->LastIndicesOfEachRail.Add(StartIndex);
		StartIndex = StartIndex + Utils::GetPerpendicularDirection(CurrentDirection);
	}
}

void UCreepRail::PrintLastIndicesOfEachRail()
{
	int32 i = 0;
	for (FIntVector LastIndex : LastIndicesOfEachRail)
	{
		UE_LOG(LogTemp, Warning, TEXT("Print LastIndex[%d] : %d %d %d"), i, LastIndex.X, LastIndex.Y, LastIndex.Z);
		i++;
	}
}

float UCreepRail::GetSlopeCreepWayRotation(const FIntVector& CurrentDirection, const FIntVector& VoxelIndex)
{
	if (CurrentDirection == FIntVector(1, 0, 1)) return 270;
	else if (CurrentDirection == FIntVector(-1, 0, 1)) return 90;
	else if (CurrentDirection == FIntVector(0, 1, 1)) return 0;
	else if (CurrentDirection == FIntVector(0, -1, 1)) return 180;
	else if (CurrentDirection == FIntVector(1, 0, -1)) return 90;
	else if (CurrentDirection == FIntVector(-1, 0, -1)) return 270;
	else if (CurrentDirection == FIntVector(0, 1, -1)) return 180;
	else if (CurrentDirection == FIntVector(0, -1, -1)) return 0;
    return 0;
}

// void UCreepRail::UpdateLastIndicesOfEachRail() // deprecated
// {
// 	// Update the LastIndicesOfEachRail
// 	for (int32 i = 0; i < this->MaxRailCount; i++)
// 	{
// 		this->LastIndicesOfEachRail[i] = this->RailBuffers[i].Last().Get<0>();
// 		UE_LOG(LogTemp, Warning, TEXT("Update LastIndex[%d] : %d %d %d"), i, LastIndicesOfEachRail[i].X, LastIndicesOfEachRail[i].Y, LastIndicesOfEachRail[i].Z);
// 	}
// }

void UCreepRail::InsertCreepWayDataRectangleIntoRailBuffers(FIntVector CurrentDirection, int32 BPActorPoolIndex, bool bRotate)
{
	// 오르막길이면 높이 하나를 낮춰줘야 한다.
	FIntVector DownOffset = (CurrentDirection.Z == 1) ? FIntVector(0, 0, -1) : FIntVector(0, 0, 0);
	
	// CurrentDirection 방향으로 직사각형 영역의 VoxelIndex를 버퍼에 로딩
	for (int32 i = 0; i < this->MaxRailCount; i++)
	{
		FIntVector VoxelIndexForRail = this->LastIndicesOfEachRail[i] + DownOffset;
		TArray<TTuple<FIntVector, int32, float, EVoxelProperty>>& RailBuffer = this->RailBuffers[i];					
		for (int32 j = 0; j < this->RailLength; j++)
		{
			// 다음 칸으로 이동 (이 코드가 맨 앞으로 와야 LastIndex 앞에 설치함)
			VoxelIndexForRail = VoxelIndexForRail + CurrentDirection;
			this->LastIndicesOfEachRail[i] = VoxelIndexForRail;
			// Up Down 유무와 방향에 맞게 SlopeCreepWayBlock을 회전시킨다.
			float Rotation = (bRotate) ? this->GetSlopeCreepWayRotation(CurrentDirection, VoxelIndexForRail) : 0;
			if (BPActorPoolIndex == 0)
				RailBuffer.Add(TTuple<FIntVector, int32, float, EVoxelProperty>(VoxelIndexForRail, BPActorPoolIndex, Rotation, EVoxelProperty::NormalCreepWay));
			else
				RailBuffer.Add(TTuple<FIntVector, int32, float, EVoxelProperty>(VoxelIndexForRail, BPActorPoolIndex, Rotation, EVoxelProperty::SlopeCreepWay));
		}
	}
}


void UCreepRail::InsertCreepWayDataTriangleIntoRailBuffers(bool bTopRailIn, const FIntVector& Direction, int32 Offset)
{
	// bTopRailIn 변수를 고려해서 CurrentDirection 방향으로 직각삼각형 영역의 VoxelIndex를 버퍼에 로딩
	for (int32 i = 0; i < this->MaxRailCount; i++)
	{
		FIntVector VoxelIndexForRailFromBottom = this->LastIndicesOfEachRail[i];
		TArray<TTuple<FIntVector, int32, float, EVoxelProperty>>& RailBufferFromBottom = this->RailBuffers[i];
		FIntVector VoxelIndexForRailFromTop = this->LastIndicesOfEachRail[this->MaxRailCount - i - 1];
		TArray<TTuple<FIntVector, int32, float, EVoxelProperty>>& RailBufferFromTop = this->RailBuffers[this->MaxRailCount - i - 1];
		for (int32 j = 0; j < i + Offset; j++)
		{
			// TopRail이 In 상태이면 TopRailBuffer부터 처리해줘야한다.
			if (bTopRailIn)
			{
				// 다음 칸으로 이동 (이 코드가 맨 앞으로 와야 LastIndex 앞에 설치함)
				VoxelIndexForRailFromTop = VoxelIndexForRailFromTop + Direction;
				RailBufferFromTop.Add(TTuple<FIntVector, int32, float, EVoxelProperty>(VoxelIndexForRailFromTop, 0, 0, EVoxelProperty::NormalCreepWay));
				this->LastIndicesOfEachRail[this->MaxRailCount - i - 1] = VoxelIndexForRailFromTop;
			}
			else
			{
				// 다음 칸으로 이동 (이 코드가 맨 앞으로 와야 LastIndex 앞에 설치함)
				VoxelIndexForRailFromBottom = VoxelIndexForRailFromBottom + Direction;
				this->LastIndicesOfEachRail[i] = VoxelIndexForRailFromBottom;
				RailBufferFromBottom.Add(TTuple<FIntVector, int32, float, EVoxelProperty>(VoxelIndexForRailFromBottom, 0, 0, EVoxelProperty::NormalCreepWay));
			}
		}
	}
}