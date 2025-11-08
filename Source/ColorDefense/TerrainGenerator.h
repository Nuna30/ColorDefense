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
public:
	UPROPERTY(EditAnywhere, Category = "Chunk Option")
    FIntVector EChunkSize = FIntVector(10, 10, 10);

	UPROPERTY(EditAnywhere, Category = "DFS Option")
	int32 ScanRadius = 1;

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
	FTransform Transform;
	TSubclassOf<AActor> BPActor;
	TWeakObjectPtr<AActor> SpawnedActor;
};

class FChunk
{
public:
	FChunk(int32 SizeX, int32 SizeY, int32 SizeZ);
	bool IsInsideChunk(const FIntVector& VoxelIdx);
	FIntVector GetEmptyIdx();
public:
	FIntVector ChunkSize;
	TArray<TArray<TArray<FVoxel>>> Chunk;
};

class FVoxelGenerator
{
public:
	FVoxelGenerator(UWorld* InWorld, FActorContainer& InActorContainer, FChunk& InChunk);
	void SetVoxelDataInChunk(const FIntVector& VoxelIdx, TSubclassOf<AActor>& BPActor, float Width, float Height);
	void DeleteVoxelDataInChunk(const FIntVector& VoxelIdx);
	void SpawnActorFromVoxel(FVoxel& Voxel);
	void DestroyActorFromVoxel(FVoxel& Voxel);
	FTransform GetWorldTransformFromVoxelIndex(const FIntVector& VoxelIdx, float Width, float Height);
public:
	UWorld* World;
	FActorContainer& ActorContainer;
	FChunk& Chunk;
	TArray<FIntVector> VoxelIdxsInChunkList;
};

class FCreepWayGenerator : public FVoxelGenerator
{
public:
	FCreepWayGenerator(UWorld* InWorld, FActorContainer& InActorContainer, FChunk& InChunk);
	bool GenerateCreepWay(int32 ScanRadius);
public:
	float CreepWayBlockWidth = 200;
	float CreepWayBlockHeight = 100;
	TArray<TArray<TArray<bool>>> Visited;
};