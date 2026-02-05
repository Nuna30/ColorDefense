// Fill out your copyright notice in the Description page of Project Settings.


#include "ChunkGeneratorManager.h"

void UChunkGeneratorManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    this->ChunkGenerator = nullptr;
}

void UChunkGeneratorManager::Deinitialize()
{
    this->ClearChunkGenerator();
    Super::Deinitialize();
}

void UChunkGeneratorManager::CreateChunkGenerator(UChunkGrid* InChunkGrid, int32 InNeiborRadius)
{
    // 1. 기존 생성기가 있다면 정리 (GC가 나중에 수거해가도록 참조 해제)
    ClearChunkGenerator();

    // 2. 새 로직 객체 생성 (NewObject)
    // Outer를 'this'(Subsystem)로 설정하여 Subsystem이 소유권을 가짐
    UChunkGenerator* NewChunkGenerator = NewObject<UChunkGenerator>(this, UChunkGenerator::StaticClass());

    // 3. 데이터 주입 및 초기화
    NewChunkGenerator->Initialize(InChunkGrid, InNeiborRadius);
    this->ChunkGenerator = NewChunkGenerator; // 멤버 변수에 저장하여 GC 방지
}

void UChunkGeneratorManager::ClearChunkGenerator()
{
    if (this->ChunkGenerator)
    {
        // 필요한 경우 여기서 Generator의 정리 함수 호출 (예: 데이터 저장 등)
        this->ChunkGenerator = nullptr; // 참조를 끊으면 GC가 다음 사이클에 수거함
    }
}