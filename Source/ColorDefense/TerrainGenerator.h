// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameEnums.h"
#include "TerrainGenerator.generated.h"

struct FVoxel; // UObject는 항상 포인터로 사용해서 크기 명시가 필요없다.
class UChunk;
class UBPActorPool;

UCLASS()
class COLORDEFENSE_API ATerrainGenerator : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ATerrainGenerator();
	void TeleportPlayerToLocation(FVector TargetLocation);
public:
	UPROPERTY(EditAnywhere, Category = "Creep Way Option")
	int32 UPMaxRailCount = 5;
	UPROPERTY(EditAnywhere, Category = "Creep Way Option")
	int32 UPRailLength = 20;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

class FVoxelGenerator
{
public:
	FVoxelGenerator(UWorld* InWorld, UBPActorPool* InBPActorPool, UChunk* InChunk);
	void SetVoxelDataInChunk(const FIntVector& VoxelIndex, int32 BPActorPoolIndex, EVoxelProperty Property);
	void DeleteVoxelDataInChunk(const FIntVector& VoxelIndex);
	void SpawnActorFromVoxel(FVoxel& Voxel);
	void DestroyActorFromVoxel(FVoxel& Voxel);
	FTransform GetWorldTransformFromVoxelIndex(const FIntVector& VoxelIndex, float Width, float Height);
public:
	UWorld* World;
	UBPActorPool* BPActorPool;
	UChunk* Chunk;
	float VoxelWidth = 200;
	float VoxelHeight = 100;
};

class FCreepWayGenerator : public FVoxelGenerator
{
public:
	FCreepWayGenerator
	(
		UWorld* InWorld,
		UBPActorPool* InBPActorPool,
		UChunk* InChunk,
		int32 MaxRailCount,
		int32 RailLength
	);
public:
	FIntVector GetPerpendicularDirection(const FIntVector& Direction);
	void DecideNextDirection();
	void UpdateTopRailIn();
	void UpdateLastIndexesOfEachRail();
	void SetLastIndexesOfEachRailToCreepCheckPoint();
	void RotateSlopeCreepWayBlock(const FIntVector& VoxelIndex);
public:
	void PrintRailBuffers();
	void PrintLastIndexes();
	void PrintDirections();
public:
	void SpawnActorWithFlushingMainBuffer();
	void FlushRailBuffersToMainBuffer();
public:
	void LoadVoxelIndexTriangleIntoRailBuffers(const FIntVector& Direction);
	void LoadVoxelIndexRectangleIntoRailBuffers(int32 BPActorPoolIndex, bool bRotate);
public:
	void InitializeCreepWay();
	void GenerateCreepWay();
public:
	void GoStraightAndUpOrDownAndGoStraight();
	void GoStraightAndTurnLeftOrRightAndGoStraight();
public:
	bool bTopRailIn;
	int32 MaxRailCount;
	int32 RailLength;
	TArray<TArray<FIntVector>> RailBuffers;
	TArray<FIntVector> MainBuffer;
	TArray<FIntVector> ODirectionArray;
	TArray<FIntVector> LastIndexesOfEachRail;
	FIntVector NextDirection;
	FIntVector CurrentDirection;
};

void print(FString DebugMessage);