// Fill out your copyright notice in the Description page of Project Settings.

#include "CreepGenerator.h"
#include "CreepPatternGeneratorManager.h"
#include "CreepPatternGenerator.h"

ACreepGenerator::ACreepGenerator()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACreepGenerator::BeginPlay()
{
	Super::BeginPlay();

    this->CreepPatternGenerator = GetGameInstance()->GetSubsystem<UCreepPatternGeneratorManager>()->CreepPatternGenerator;
}

void ACreepGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACreepGenerator::GenerateCreeps()
{
    if (this->CreepPatternGenerator->IsEmpty(this->RailNumber)) this->CreepPatternGenerator->GeneratePattern();
    FSpawnInfo SpawnInfo = this->CreepPatternGenerator->PopSpawnInfo(this->RailNumber);

    SpawnCreep(SpawnInfo.CreepColorNumber);

    GetWorld()->GetTimerManager().SetTimer(
		this->CreepGenerationTimer,         // 타이머 핸들
		this,                // 실행할 대상 객체
		&ACreepGenerator::GenerateCreeps, // 실행할 함수 포인터
		SpawnInfo.SpawnDelay,                // 주기 (초 단위)
		false                 // 반복 여부 (true면 무한 반복)
	);
}

void ACreepGenerator::SpawnCreep(int32 ColorNumber)
{
	// 스폰할 위치와 회전 정보
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = FRotator::ZeroRotator;
	FTransform SpawnTransform(SpawnRotation, SpawnLocation);

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
    switch (ColorNumber) {
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