// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameEnums.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CreepWayHandler.generated.h"

UCLASS()
class COLORDEFENSE_API UCreepWayHandler : public UObject
{
	GENERATED_BODY()
public:
	UCreepWayHandler();
    void Initialize();
private:
	class UChunkGenerator* ChunkGenerator;
	class UCreepWayGenerator* CreepWayGenerator;
public:
	void BuildCreepWay();
	void DestructCreepWay();
};