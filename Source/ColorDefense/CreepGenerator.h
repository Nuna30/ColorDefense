// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/GameplayStatics.h" // 지연 스폰을 사용하기 위해
#include "Creep.h" // 크립을 생성하기 위해
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CreepGenerator.generated.h"

UCLASS()
class COLORDEFENSE_API ACreepGenerator : public AActor
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditAnywhere, Category="Setting")
	TSubclassOf<ACreep> CreepClass;

	UPROPERTY()
	int32 RailNumber;

	UPROPERTY()
	FTimerHandle CreepGenerationTimer;

	UPROPERTY()
	class UCreepPatternGenerator* CreepPatternGenerator;
public:	
	ACreepGenerator();
	virtual void Tick(float DeltaTime) override;
	void SetRailNumber(int32 InRailNumber);
	void SpawnCreep(int32 ColorNumbe);
	void GenerateCreeps();
};
