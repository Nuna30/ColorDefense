// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TerrainGenerator.generated.h"

enum class EVoxelProperty : int32;

UCLASS()
class COLORDEFENSE_API ATerrainGenerator : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ATerrainGenerator();
	void TeleportPlayerToLocation(FVector TargetLocation);
public:
	UPROPERTY(EditAnywhere, Category = "Chunk Option")
    FIntVector UPChunkSize = FIntVector(10, 10, 10);
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

class FVoxel 
{
public:
	FVoxel();
public:
	FIntVector Index;
	EVoxelProperty Property;
	FTransform Transform;
	TSubclassOf<AActor> BPActor;
	TWeakObjectPtr<AActor> SpawnedActor;
};

class FChunk
{
public:
	FChunk(int32 SizeX, int32 SizeY, int32 SizeZ);
	void ExpandChunk(const FIntVector& VoxelIndex);
	bool IsInsideChunk(const FIntVector& VoxelIndex);
	bool IsEmptyIndex(const FIntVector& VoxelIndex);
	void SetRotation(const FIntVector& VoxelIndex, float Rotation);
public:
	FIntVector ChunkSize;
	TArray<TArray<TArray<FVoxel>>> Chunk;
};

class FVoxelGenerator
{
public:
	FVoxelGenerator(UWorld* InWorld, class UBPActorPool* InBPActorPool, FChunk& InChunk);
	void SetVoxelDataInChunk(const FIntVector& VoxelIndex, int32 BPActorPoolIndex, EVoxelProperty Property);
	void DeleteVoxelDataInChunk(const FIntVector& VoxelIndex);
	void SpawnActorFromVoxel(FVoxel& Voxel);
	void DestroyActorFromVoxel(FVoxel& Voxel);
	FTransform GetWorldTransformFromVoxelIndex(const FIntVector& VoxelIndex, float Width, float Height);
public:
	UWorld* World;
	class UBPActorPool* BPActorPool;
	FChunk& Chunk;
	float VoxelWidth = 200;
	float VoxelHeight = 100;
};

class FCreepWayGenerator : public FVoxelGenerator
{
public:
	FCreepWayGenerator
	(
		UWorld* InWorld,
		class UBPActorPool* InBPActorPool,
		FChunk& InChunk,
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