// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "BPActorPool.generated.h"

/**
 * 
 */
UCLASS()
class COLORDEFENSE_API UBPActorPool : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	TArray<TSubclassOf<AActor>> Pool;
	TArray<FString> BPActorPathPool =
	{
		"/Game/Blueprints/Actors/BP_Grass_Plain.BP_Grass_Plain_C",
		"/Game/Blueprints/Actors/BP_Grass_Slope.BP_Grass_Slope_C",
		"/Game/Blueprints/Actors/BP_WayPoint.BP_WayPoint_C"
	};
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};