// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CreepPatternGenerator.generated.h"

USTRUCT()
struct FSpawnInfo
{
	GENERATED_BODY()

    UPROPERTY()
    int32 CreepColorNumber;

    UPROPERTY()
    float SpawnDelay;
};

UCLASS()
class COLORDEFENSE_API UCreepPatternGenerator : public UObject
{
	GENERATED_BODY()
public:
	int32 MaxRailCount;
	TArray<TArray<FSpawnInfo>> PatternArray;
	float SpawnDelay = 1;
	int32 PropagationProbability = 50;

public:
	UCreepPatternGenerator();
	void Initialize(int32 InMaxRailCount);
	bool IsEmpty(int32 RailNumber);
	FSpawnInfo PopSpawnInfo(int32 RailNumber);
	void GeneratePattern();
	void FillPatternArray(int32 Count, int32 CreepColorNumber);
	void ChangeCreepColorNumberUsingBFS(int32 Row, int32 Col, int32 NewCreepColorNumber, int32 Prob);
	void ChangeCreepColorNumberUsingDFS(int32 Row, int32 Col, int32 NewCreepColorNumber, int32 PropagationCount);
};
