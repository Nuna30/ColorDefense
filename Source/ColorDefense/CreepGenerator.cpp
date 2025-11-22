// Fill out your copyright notice in the Description page of Project Settings.


#include "CreepGenerator.h"
#include "Creep.h"
#include "CreepPoolSubsystem.h"
#include "GameEnums.h"

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

	// 1초마다 크립 생성
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
	
	// int RandomColor = FMath::RandRange(1, 7);
	// switch (RandomColor) {
	// 	case 1 : Creep->ChangeColor(EColor::Red); break;
	// 	case 2 : Creep->ChangeColor(EColor::Orange); break;
	// 	case 3 : Creep->ChangeColor(EColor::Yellow); break;
	// 	case 4 : Creep->ChangeColor(EColor::Green); break;
	// 	case 5 : Creep->ChangeColor(EColor::Blue); break;
	// 	case 6 : Creep->ChangeColor(EColor::Purple); break;
	// 	case 7 : Creep->ChangeColor(EColor::Indigo); break;
	// }
}
