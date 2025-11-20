// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameEnums.h"
#include "TerrainGenerator.generated.h"

struct FVoxel; // UObject는 항상 포인터로 사용해서 크기 명시가 필요없다.
class UChunk;
class UBPActorPool;
class FVoxelGenerator;

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
