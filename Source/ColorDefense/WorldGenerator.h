// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CreepWayGeneratorManager.h" // 월드에 CreepWay을 생성하기 위해
#include "CreepCheckPointGeneratorManager.h" // 코너마다 CreepCheckPoint를 생성하기 위해
#include "CreepGeneratorGeneratorManager.h" // 크립을 생성하기 위해
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldGenerator.generated.h"

UCLASS()
class COLORDEFENSE_API AWorldGenerator : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Setting")
	int32 UPMaxRailCount = 5;
	UPROPERTY(EditAnywhere, Category = "Setting")
	int32 UPRailLength = 5; // 크면 배열 넘어가버림

public:	
	// Sets default values for this actor's properties
	AWorldGenerator();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void TeleportPlayerToLocation(FVector TargetLocation);
};
