#include "WorldGenerator.h"

#define DEBUGMODE 1

// ----------------------------------------------------- AWorldGenerator ---------------------------------------------- //

AWorldGenerator::AWorldGenerator()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AWorldGenerator::BeginPlay()
{
	Super::BeginPlay();

	// ------------------------------------ 클래스 초기화 ---------------------------------------//
	UWorld* World = GetWorld(); // 액터를 스폰하기 위해선 UWorld 객체가 필요하다.
	UVoxelGrid* VoxelGrid = World->GetSubsystem<UVoxelGrid>(); VoxelGrid->VoxelGridSize = this->VoxelGridSize; VoxelGrid->InitVoxelGrid();
    UChunkGrid* ChunkGrid = World->GetSubsystem<UChunkGrid>(); ChunkGrid->ChunkGridSize = this->ChunkGridSize; ChunkGrid->InitChunkGrid();
	UGameInstance* GameInstance = GetGameInstance();
	UBPActorPool* BPActorPool = GameInstance->GetSubsystem<UBPActorPool>();
    UChunkGeneratorManager* ChunkGeneratorManager = GameInstance->GetSubsystem<UChunkGeneratorManager>();
	UCreepWayGeneratorManager* CreepWayGeneratorManager = GameInstance->GetSubsystem<UCreepWayGeneratorManager>();
    UCreepCheckPointGeneratorManager* CreepCheckPointGeneratorManager = GameInstance->GetSubsystem<UCreepCheckPointGeneratorManager>();
    UCreepGeneratorGeneratorManager* CreepGeneratorGeneratorManager = GameInstance->GetSubsystem<UCreepGeneratorGeneratorManager>();

    // ------------------------------------ 로직 클래스 초기화 -----------------------------------//
    ChunkGeneratorManager->CreateChunkGenerator(ChunkGrid);
    ChunkGeneratorManager->ChunkGenerator->GenerateCreepWayChunk(this->ChunkCount, NeighborRadius);
    CreepCheckPointGeneratorManager->CreateCreepCheckPointGenerators(World, BPActorPool, VoxelGrid, MaxRailCount);
    TArray<UCreepCheckPointGenerator*>& CreepCheckPointGenerators = CreepCheckPointGeneratorManager->CreepCheckPointGenerators;
    CreepWayGeneratorManager->CreateCreepWayGenerator(World, BPActorPool, VoxelGrid, ChunkGrid, CreepCheckPointGenerators, MaxRailCount, RailLength, ChunkGeneratorManager->ChunkGenerator->DirectionContainer);
    CreepGeneratorGeneratorManager->CreateCreepGeneratorGenerator(World, BPActorPool, VoxelGrid);
    
	// -------------------------------------- 테스트 -------------------------------------------//
    UCreepWayGenerator* CreepWayGenerator = CreepWayGeneratorManager->CreepWayGenerator;
	CreepWayGenerator->GenerateCreepWay(this->GenerationStep);
    UCreepGeneratorGenerator* CreepGeneratorGenerator = CreepGeneratorGeneratorManager->CreepGeneratorGenerator;
    // CreepGenerator 설치
    FIntVector StartIndex = FIntVector(VoxelGrid->VoxelGridSize.X / 2, VoxelGrid->VoxelGridSize.Y / 2, VoxelGrid->VoxelGridSize.Z / 2);
    for (int32 i = 0; i < MaxRailCount; i++)
    {
        CreepGeneratorGenerator->CreateCreepGenerator(StartIndex + FIntVector(1, i, 2), i);
    }
    // 텔포
	TeleportPlayerToLocation
    (
        FVector
        (
            (VoxelGrid->VoxelGridSize.X / 2 + 1) * CreepWayGenerator->VoxelWidth,
            (VoxelGrid->VoxelGridSize.Y / 2 + 1) * CreepWayGenerator->VoxelWidth,
            VoxelGrid->VoxelGridSize.Z / 2 * CreepWayGenerator->VoxelHeight + 2500
        )
    );
}

// Called every frame
void AWorldGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWorldGenerator::TeleportPlayerToLocation(FVector TargetLocation)
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
