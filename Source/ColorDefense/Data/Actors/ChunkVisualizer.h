// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChunkVisualizer.generated.h"

UCLASS()
class COLORDEFENSE_API AChunkVisualizer : public AActor
{
	GENERATED_BODY()
	AChunkVisualizer();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public: // --- Components --- //
	class UChunkGenerator* ChunkGenerator;

public:	// --- Bind delegate --- //
	UFUNCTION()
	void OnChunkGenerated();

	UFUNCTION()
	void OnChunkDeleted();

public: // --- Visualization --- //
	FIntVector SpawnOffset;
	int32 VisualBlockWidth = 250 / 2;
	int32 VisualBlockHeight = 125 / 2;
	FString VisualBlockPath;
	TSubclassOf<AActor> VisualBlock;

	TArray<AActor*> VisualBlockContainer;
	void VisualizeStartingPoint();
	void GenerateVisualBlocks();
	void DeleteVisualBlocks();

public: // --- Utils --- //
	void SetStartingLocation(FIntVector StartingLocation);
};
