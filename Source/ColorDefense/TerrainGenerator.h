// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameEnums.h"
#include "TerrainGenerator.generated.h"

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

class FActorContainer
{
public:
	TArray<TSubclassOf<AActor>> Container;
	TArray<FString> ActorPathContainer =
	{
		"/Game/Blueprints/Actors/BP_Grass_Plain.BP_Grass_Plain_C",
		"/Game/Blueprints/Actors/BP_Grass_Slope.BP_Grass_Slope_C"
	};
public:
	FActorContainer();
	void LoadActors();
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
	FVoxelGenerator(UWorld* InWorld, FActorContainer& InActorContainer, FChunk& InChunk);
	void SetVoxelDataInChunk(const FIntVector& VoxelIndex, int32 ActorContainerIndex, EVoxelProperty Property);
	void DeleteVoxelDataInChunk(const FIntVector& VoxelIndex);
	void SpawnActorFromVoxel(FVoxel& Voxel);
	void DestroyActorFromVoxel(FVoxel& Voxel);
	FTransform GetWorldTransformFromVoxelIndex(const FIntVector& VoxelIndex, float Width, float Height);
public:
	UWorld* World;
	FActorContainer& ActorContainer;
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
		FActorContainer& InActorContainer,
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
	void LoadVoxelIndexRectangleIntoRailBuffers(int32 ActorContainerIndex, bool bRotate);
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

class FCreepCheckPointGenerator
{
public:
	FCreepCheckPointGenerator
	(
		FChunk& InChunk
	);
	void GenerateCreepCheckPoint();
public:
	FChunk& Chunk;
};

void print(FString DebugMessage);