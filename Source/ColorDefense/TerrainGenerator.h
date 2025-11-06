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

	const int32 MapX = 30;
	const int32 MapY = 30;
	const int32 MapZ = 3;
	TArray<TArray<int32>> HeightMap;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GenerateHeightMap(int32 MaxHeight);
	void GenerateTerrain();
	void GenerateEnvironment();
	void PlaceEnvironment(const TArray<FString>& ObjPathList, int32 Percent, int32 OffsetZ, const FVector& BlockSize, const FRotator& Rotation, const FVector& Scale);
	void SpawnStaticMeshByPath(const FSoftObjectPath& MeshPath, const FVector& Location, const FRotator& Rotation, const FVector& Scale, bool bCollision);
	void TeleportPlayerToLocation(FVector TargetLocation);
	void RangeIncrementBy1(int32 x, int32 y, int32 r);

};

class Voxel 
{
public:
	Voxel();
private:
	FTransform Transform;
	TSubclassOf<AActor> BPClass;
	EAssetType AssetType;
	EArea Area;
};