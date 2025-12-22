// Fill out your copyright notice in the Description page of Project Settings.


#include "CreepGenerator.h"

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
	FTimerHandle TimerHandle;
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
	FTransform SpawnTransform(SpawnRotation, SpawnLocation);

	if (CreepClass == nullptr) return;

	// 1. 지연 스폰 시작 (아직 BeginPlay가 실행되지 않은 상태로 객체만 생성됨)
    ACreep* Creep = GetWorld()->SpawnActorDeferred<ACreep>(
        CreepClass, 
        SpawnTransform, 
        this, // Owner
        nullptr, // Instigator
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn
    );

    // 2. BeginPlay 이전에 변수 설정! (이제 안전하게 설정 가능)
    Creep->RailNumber = this->RailNumber;

    // 색상 설정 로직도 미리 처리 가능
    int RandomColor = FMath::RandRange(1, 7);
    switch (RandomColor) {
        case 1 : Creep->ChangeColor(EColor::Red); break;
        case 2 : Creep->ChangeColor(EColor::Orange); break;
        case 3 : Creep->ChangeColor(EColor::Yellow); break;
        case 4 : Creep->ChangeColor(EColor::Green); break;
        case 5 : Creep->ChangeColor(EColor::Blue); break;
        case 6 : Creep->ChangeColor(EColor::Purple); break;
        case 7 : Creep->ChangeColor(EColor::Indigo); break;
    }

    // 3. 스폰 마무리 (이때 BeginPlay가 실행됨)
    UGameplayStatics::FinishSpawningActor(Creep, SpawnTransform);
}

void ACreepGenerator::SetRailNumber(int32 InRailNumber)
{
	this->RailNumber = InRailNumber;
}