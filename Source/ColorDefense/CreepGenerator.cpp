// Fill out your copyright notice in the Description page of Project Settings.


#include "CreepGenerator.h"
#include "Creep.h"

// Sets default values
ACreepGenerator::ACreepGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACreepGenerator::BeginPlay()
{
	Super::BeginPlay();

	GenerateCreep(1);
	
}

// Called every frame
void ACreepGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACreepGenerator::GenerateCreep(float Period)
{
	GetWorldTimerManager().SetTimer(
		TimerHandle,         // 타이머 핸들
		this,                // 실행할 대상 객체
		&ACreepGenerator::SpawnCreep, // 실행할 함수 포인터
		Period,                // 주기 (초 단위)
		true                 // 반복 여부 (true면 무한 반복)
	);
}

void ACreepGenerator::SpawnCreep()
{
	// 스폰할 위치와 회전 정보
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = FRotator::ZeroRotator;

	if (CreepClass == nullptr) return;

	// 월드에 크립 스폰
	ACreep* Creep = GetWorld()->SpawnActor<ACreep>(CreepClass, SpawnLocation, SpawnRotation);

	// GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, FString::Printf(TEXT("Creep Spawned.")));

	// if (Creep == nullptr) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Creep is null")));
}
