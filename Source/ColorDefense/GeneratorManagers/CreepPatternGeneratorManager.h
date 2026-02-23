// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CreepPatternGeneratorManager.generated.h"

UCLASS()
class COLORDEFENSE_API UCreepPatternGeneratorManager : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	void CreateCreepPatternGenerator(int32 InMaxRailCount);
	void ClearCreepPatternGenerator();
public:
	UPROPERTY()
	class UCreepPatternGenerator* CreepPatternGenerator;
};
