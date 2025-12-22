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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// 생성할 크립을 에디터에서 고를 수 있음 (기본 BP_Creep)
	UPROPERTY(EditAnywhere, Category="Setting")
	TSubclassOf<ACreep> CreepClass;
	// 어느 레일에 속하는지
	int32 RailNumber;
public:	
	// Sets default values for this actor's properties
	ACreepGenerator();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Period 마다 크립 생성하는 함수
	void GenerateCreep(float Period);
	// 크립 스폰하는 함수  
	void SpawnCreep();
	// 외부에서 RailNumber를 설정할 수 있도록
	void SetRailNumber(int32 InRailNumber);
};
