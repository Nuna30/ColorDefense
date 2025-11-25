#include "TerrainGenerator.h"
#include "BPActorPool.h"
#include "Chunk.h"
#include "Voxel.h"
#include "CreepWayGeneratorManager.h"
#include "CreepCheckPointGeneratormanager.h"

#define DEBUGMODE 1

// ----------------------------------------------------- ATerrainGenerator ---------------------------------------------- //

ATerrainGenerator::ATerrainGenerator()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATerrainGenerator::BeginPlay()
{
	Super::BeginPlay();

	// ------------------------------------ 클래스 초기화 ---------------------------------------//
	UWorld* World = GetWorld(); // 액터를 스폰하기 위해선 UWorld 객체가 필요하다.
	UChunk* Chunk = World->GetSubsystem<UChunk>();
	UGameInstance* GameInstance = GetGameInstance();
	UBPActorPool* BPActorPool = GameInstance->GetSubsystem<UBPActorPool>();
	UCreepWayGeneratorManager* CreepWayGeneratorManager = GameInstance->GetSubsystem<UCreepWayGeneratorManager>();
    UCreepCheckPointGeneratorManager* CreepCheckPointGeneratorManager = GameInstance->GetSubsystem<UCreepCheckPointGeneratorManager>();

    // ------------------------------------ 로직 클래스 초기화 -----------------------------------//
    CreepCheckPointGeneratorManager->CreateCreepCheckPointGenerators(World, BPActorPool, Chunk, UPMaxRailCount);
    TArray<UCreepCheckPointGenerator*>& CreepCheckPointGenerators = CreepCheckPointGeneratorManager->CreepCheckPointGenerators;
    CreepWayGeneratorManager->CreateCreepWayGenerator(World, BPActorPool, Chunk, CreepCheckPointGenerators, UPMaxRailCount, UPRailLength);

	// -------------------------------------- 테스트 -------------------------------------------//
    UCreepWayGenerator* CreepWayGenerator = CreepWayGeneratorManager->CreepWayGenerator;
	CreepWayGenerator->GenerateCreepWay();
	TeleportPlayerToLocation
    (
        FVector
        (
            (Chunk->ChunkSize.X / 2 + 1) * CreepWayGenerator->VoxelWidth,
            (Chunk->ChunkSize.Y / 2 + 1) * CreepWayGenerator->VoxelWidth,
            Chunk->ChunkSize.Z / 2 * CreepWayGenerator->VoxelHeight + 2500
        )
    );
}

// Called every frame
void ATerrainGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATerrainGenerator::TeleportPlayerToLocation(FVector TargetLocation)
{
    // 1. 현재 월드를 가져옵니다.
    UWorld* World = GetWorld();
    if (!World) return;

    // 2. 현재 플레이어 컨트롤러를 가져옵니다.
    APlayerController* PlayerController = World->GetFirstPlayerController();
    if (!PlayerController) return;

    // 3. 컨트롤러가 빙의(Possess)하고 있는 폰/캐릭터를 가져옵니다.
    APawn* PlayerPawn = PlayerController->GetPawn();
    if (!PlayerPawn) return;

    // 4. 순간이동 함수 호출
    PlayerPawn->TeleportTo(
        TargetLocation, 
        PlayerPawn->GetActorRotation(), // 현재 회전 유지
        false,                          // bIsATest: false (실제 이동)
        true                            // bNoCheck: true (충돌 검사 없이 강제 이동)
    );
}
