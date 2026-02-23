// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Generators/ChunkGenerator.h" // ChunkGenerator를 관리하기 위해
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ChunkGeneratorManager.generated.h"

UCLASS()
class COLORDEFENSE_API UChunkGeneratorManager : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	void CreateChunkGenerator(UChunkGrid* InChunkGrid, int32 InNeigborRadius);
	void ClearChunkGenerator();
public:
	UPROPERTY()
	UChunkGenerator* ChunkGenerator;
};
