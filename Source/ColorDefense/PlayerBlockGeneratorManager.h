// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerBlockGenerator.h" // PlayerBlockGenerator를 관리하기 위해
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PlayerBlockGeneratorManager.generated.h"

UCLASS()
class COLORDEFENSE_API UPlayerBlockGeneratorManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	void CreatePlayerBlockGenerator
	(
		UWorld* InWorld, 
		UBPActorPool* InBPActorPool, 
		UVoxelGrid* InVoxelGrid, 
		float InVoxelWidth,
		float InVoxelHeight
	);
	void ClearPlayerBlockGenerator();
public:
	UPROPERTY()
	UPlayerBlockGenerator* PlayerBlockGenerator;
};
