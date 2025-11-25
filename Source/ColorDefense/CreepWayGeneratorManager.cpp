// Fill out your copyright notice in the Description page of Project Settings.


#include "CreepWayGeneratorManager.h"
#include "CreepWayGenerator.h"
#include "BPActorPool.h"
#include "Chunk.h"

void UCreepWayGeneratorManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    this->CreepWayGenerator = nullptr;
}

void UCreepWayGeneratorManager::Deinitialize()
{
    this->ClearCreepWayGenerator();
    Super::Deinitialize();
}

void UCreepWayGeneratorManager::CreateCreepWayGenerator(UWorld* InWorld, UBPActorPool* InBPActorPool, UChunk* InChunk, TArray<UCreepCheckPointGenerator*>& InCreepCheckPointGenerators, int32 InMaxRailCount, int32 InRailLength)
{
    // 1. 기존 생성기가 있다면 정리 (GC가 나중에 수거해가도록 참조 해제)
    ClearCreepWayGenerator();

    // 2. 새 로직 객체 생성 (NewObject)
    // Outer를 'this'(Subsystem)로 설정하여 Subsystem이 소유권을 가짐
    UCreepWayGenerator* NewCreepWayGenerator = NewObject<UCreepWayGenerator>(this, UCreepWayGenerator::StaticClass());

    // 3. 데이터 주입 및 초기화
    NewCreepWayGenerator->Initialize(InWorld, InBPActorPool, InChunk, InCreepCheckPointGenerators, InMaxRailCount, InRailLength);
    this->CreepWayGenerator = NewCreepWayGenerator; // 멤버 변수에 저장하여 GC 방지
}

void UCreepWayGeneratorManager::ClearCreepWayGenerator()
{
    if (this->CreepWayGenerator)
    {
        // 필요한 경우 여기서 Generator의 정리 함수 호출 (예: 데이터 저장 등)
        this->CreepWayGenerator = nullptr; // 참조를 끊으면 GC가 다음 사이클에 수거함
    }
}