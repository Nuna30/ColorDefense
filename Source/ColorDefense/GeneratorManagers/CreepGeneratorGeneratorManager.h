// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Generators/CreepGeneratorGenerator.h" // 크립생성기생성기를 관리하기 위해
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CreepGeneratorGeneratorManager.generated.h"

UCLASS()
class COLORDEFENSE_API UCreepGeneratorGeneratorManager : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UPROPERTY()
	UCreepGeneratorGenerator* CreepGeneratorGenerator;
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
public:
	void CreateCreepGeneratorGenerator(UWorld* InWorld, UBPActorPool* InBPActorPool, UVoxelGrid* InVoxelGrid, float InVoxelWidth, float InVoxelHeight);
	void ClearCreepGeneratorGenerator();
};