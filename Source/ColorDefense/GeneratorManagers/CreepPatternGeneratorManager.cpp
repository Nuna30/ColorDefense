#include "Generators/CreepPatternGenerator.h" // CreepPatternGenerator를 관리하기 위해
#include "CreepPatternGeneratorManager.h"

void UCreepPatternGeneratorManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    this->CreepPatternGenerator = nullptr;
}

void UCreepPatternGeneratorManager::Deinitialize()
{
    this->ClearCreepPatternGenerator();
    Super::Deinitialize();
}

void UCreepPatternGeneratorManager::CreateCreepPatternGenerator(int32 InMaxRailCount)
{
    // 1. 기존 생성기가 있다면 정리 (GC가 나중에 수거해가도록 참조 해제)
    ClearCreepPatternGenerator();

    // 2. 새 로직 객체 생성 (NewObject)
    // Outer를 'this'(Subsystem)로 설정하여 Subsystem이 소유권을 가짐
    UCreepPatternGenerator* NewCreepPatternGenerator = NewObject<UCreepPatternGenerator>(this, UCreepPatternGenerator::StaticClass());

    // 3. 데이터 주입 및 초기화
    NewCreepPatternGenerator->Initialize(InMaxRailCount);
    this->CreepPatternGenerator = NewCreepPatternGenerator; // 멤버 변수에 저장하여 GC 방지
}

void UCreepPatternGeneratorManager::ClearCreepPatternGenerator()
{
    if (this->CreepPatternGenerator)
    {
        // 필요한 경우 여기서 Generator의 정리 함수 호출 (예: 데이터 저장 등)
        this->CreepPatternGenerator = nullptr; // 참조를 끊으면 GC가 다음 사이클에 수거함
    }
}