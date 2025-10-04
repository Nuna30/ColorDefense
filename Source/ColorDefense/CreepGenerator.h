// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CreepGenerator.generated.h"

UCLASS()
class COLORDEFENSE_API ACreepGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACreepGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// 타이머
	FTimerHandle TimerHandle;

	// Period 마다 크립 생성하는 함수
	void GenerateCreep(float Period);
	
	// 크립 스폰하는 함수  
	void SpawnCreep();

	// 생성할 크립을 에디터에서 고를 수 있음
	UPROPERTY(EditAnywhere, Category="Setting")
	TSubclassOf<class ACreep> CreepClass;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
