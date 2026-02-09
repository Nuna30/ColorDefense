// Fill out your copyright notice in the Description page of Project Settings.

#include "VoxelGenerator.h"

#define DEBUGMODE 1

UVoxelGenerator::UVoxelGenerator()
{
}

void UVoxelGenerator::Initialize(UWorld* InWorld, UBPActorPool* InBPActorPool, UVoxelGrid* InVoxelGrid, float InVoxelWidth, float InVoxelHeight)
{
    // The Reason the goat climbs the mountain is its stubbornness.
    this->World = InWorld;
    this->BPActorPool = InBPActorPool;
    this->VoxelGrid = InVoxelGrid;
	this->VoxelWidth = InVoxelWidth;
	this->VoxelHeight = InVoxelHeight;
}

void UVoxelGenerator::SetVoxelDataInVoxelGrid(const FIntVector& VoxelIndex, int32 BPActorPoolIndex, float Rotation, EVoxelProperty Property)
{
	// VoxelIndex가 VoxelGrid 범위 안에 있는지 체크
	if (!VoxelGrid->IsInsideVoxelGrid(VoxelIndex))
	{
		if (DEBUGMODE) 
		{
			FString DebugMessage = FString::Printf(TEXT("UVoxelGenerator::SetVoxelDataInVoxelGrid {Voxel Index Not Inside VoxelGrid}"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, DebugMessage);
			UE_LOG(LogTemp, Error, TEXT("%s"), *DebugMessage);
		}
		return;
	}

	if (BPActorPoolIndex > BPActorPool->Pool.Num() - 1)
	{
		if (DEBUGMODE) 
		{
			FString DebugMessage = FString::Printf(TEXT("UVoxelGenerator::SetVoxelDataInVoxelGrid {ActorContainer out of index}"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, DebugMessage);
			UE_LOG(LogTemp, Error, TEXT("%s"), *DebugMessage);
		}
		return;
	}

	// 설정할 Voxel 레퍼런스 얻기
	FVoxel& TargetVoxel = VoxelGrid->VoxelGrid[VoxelIndex.X][VoxelIndex.Y][VoxelIndex.Z];

	// Voxel 설정
	TargetVoxel.Transform = GetWorldTransformFromVoxelIndex(VoxelIndex);
	TargetVoxel.BPActor = BPActorPool->Pool[BPActorPoolIndex];
	TargetVoxel.Property = Property;
	TargetVoxel.Index = VoxelIndex;
	this->VoxelGrid->SetRotation(VoxelIndex, Rotation);
}

void UVoxelGenerator::DeleteVoxelDataInVoxelGrid(const FIntVector& VoxelIndex)
{
	// VoxelIndex가 VoxelGrid 범위 안에 있는지 체크
	if (!VoxelGrid->IsInsideVoxelGrid(VoxelIndex))
	{
		if (DEBUGMODE) 
		{
			FString DebugMessage = FString::Printf(TEXT("UVoxelGenerator::DeleteVoxelDataInVoxelGrid {Voxel Index Not Inside VoxelGrid}"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, DebugMessage);
			UE_LOG(LogTemp, Error, TEXT("%s"), *DebugMessage);
		}
		return;
	}

	// 설정할 Voxel 레퍼런스 얻기
	FVoxel& TargetVoxel = VoxelGrid->VoxelGrid[VoxelIndex.X][VoxelIndex.Y][VoxelIndex.Z];

	// Voxel 설정
	TargetVoxel.Transform = FTransform::Identity;
	TargetVoxel.BPActor = nullptr;
	TargetVoxel.Property = EVoxelProperty::Empty;
	TargetVoxel.Index = FIntVector::ZeroValue;
}

AActor* UVoxelGenerator::SpawnActorFromVoxel(FVoxel& Voxel)
{
	// The BPActor might be nullptr sometimes.
	if (!Voxel.BPActor) 
    {
        UE_LOG(LogTemp, Warning, TEXT("BPActor is NULL"));
        return nullptr;
    }

	// 레벨에 복셀 기반 액터 소환
	AActor* NewActor = World -> SpawnActor<AActor> (
		Voxel.BPActor,
		Voxel.Transform.GetLocation(),
		Voxel.Transform.Rotator()
	);

	// Voxel에 저장
	Voxel.SpawnedActor = NewActor;

	return NewActor;
}

void UVoxelGenerator::DestroyActorFromVoxel(FVoxel& Voxel)
{
	AActor* ActorToDestroy = Voxel.SpawnedActor.Get();

 	if (IsValid(ActorToDestroy))
 	{
 		ActorToDestroy->Destroy();
 	}
    
 	Voxel.SpawnedActor = nullptr;
	Voxel.Property = EVoxelProperty::Empty;
	Voxel.BPActor = nullptr;
}

FTransform UVoxelGenerator::GetWorldTransformFromVoxelIndex(const FIntVector& VoxelIndex)
{
	float x = VoxelIndex.X;
	float y = VoxelIndex.Y;
	float z = VoxelIndex.Z;

	x *= VoxelWidth;
	y *= VoxelWidth;
	z *= VoxelHeight;

	FVector WorldLocation = FVector(x, y, z);
	return FTransform(FQuat(FRotator(0, 0, 0)), WorldLocation, FVector(VoxelWidth / 125));
}