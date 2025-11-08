#include "TerrainGenerator.h"
#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/UObjectGlobals.h"

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
	FActorContainer ActorContainer = FActorContainer();
	ActorContainer.LoadActors();
	FChunk Chunk = FChunk(EChunkSize.X, EChunkSize.Y, EChunkSize.Z);
	UWorld* World = GetWorld(); // 액터를 스폰하기 위해선 UWorld 객체가 필요하다.
	FCreepWayGenerator CreepWayGenerator = FCreepWayGenerator(World, ActorContainer, Chunk);

	// -------------------------------------- 테스트 -------------------------------------------//
	while(!CreepWayGenerator.GenerateCreepWay(ScanRadius));
}

// Called every frame
void ATerrainGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// ------------------------------------------------- FActorContainer ---------------------------------------------------- //

FActorContainer::FActorContainer()
{
}

void FActorContainer::LoadActors()
{
	for (FString ActorPath : ActorPathContainer)
	{
		Container.Add(LoadClass<AActor>(nullptr, *ActorPath));
		if (!Container.Top()) 
		{
			if (DEBUGMODE)
			{
				FString DebugMessage = FString::Printf(TEXT("FActorContainer::LoadActors {Load Failed.}"));
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, DebugMessage);
				UE_LOG(LogTemp, Error, TEXT("%s"), *DebugMessage);
			}
			return;
		}
	}
}

// ------------------------------------------------- FVoxel ------------------------------------------------------------- //

FVoxel::FVoxel()
{
	this->Transform = FTransform::Identity;
	this->BPActor = nullptr;
}

// ------------------------------------------------- FChunk ------------------------------------------------------------- //

FChunk::FChunk(int32 ChunkSizeX, int32 ChunkSizeY, int32 ChunkSizeZ)
{
	// Size 초기화
	this->ChunkSize = FIntVector(ChunkSizeX, ChunkSizeY, ChunkSizeZ);
	
	// Chunk 초기화
	Chunk.SetNum(ChunkSizeX);
	for (int32 X = 0; X < ChunkSizeX; X++)
	{
		Chunk[X].SetNum(ChunkSizeY);
		for(int32 Y = 0; Y < ChunkSizeY; Y++)
		{
			Chunk[X][Y].SetNum(ChunkSizeZ);
		}
	}
}

bool FChunk::IsInsideChunk(const FIntVector& VoxelIdx)
{
	int32 x = VoxelIdx.X;
	int32 y = VoxelIdx.Y;
	int32 z = VoxelIdx.Z;

	bool InsideChunk = true;

	if (!(x >= 0 && x < this->ChunkSize.X)) InsideChunk = false;
	if (!(y >= 0 && y < this->ChunkSize.Y)) InsideChunk = false;
	if (!(z >= 0 && z < this->ChunkSize.Z)) InsideChunk = false;

	return InsideChunk;
}

FIntVector FChunk::GetEmptyIdx()
{
	int32 x = FMath::RandRange(0, ChunkSize.X - 1);
	int32 y = FMath::RandRange(0, ChunkSize.Y - 1);
	int32 z = FMath::RandRange(0, ChunkSize.Z - 1);

	int32 count = 0;

	while (Chunk[x][y][z].BPActor)
	{
		x = FMath::RandRange(0, ChunkSize.X - 1);
		y = FMath::RandRange(0, ChunkSize.Y - 1);
		z = FMath::RandRange(0, ChunkSize.Z - 1);
		count++;

		if (count == ChunkSize.X * ChunkSize.Y * ChunkSize.Z) 
		{
			if (DEBUGMODE) 
			{
				FString DebugMessage = FString::Printf(TEXT("FChunk::GetEmptyIdx {Infinite Loop!!!}"));
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, DebugMessage);
				UE_LOG(LogTemp, Error, TEXT("%s"), *DebugMessage);
			}
			return FIntVector::ZeroValue;
		}
	}

	return FIntVector(x, y, z);
}

// ------------------------------------------------- FVoxelGenerator ---------------------------------------------------- //

FVoxelGenerator::FVoxelGenerator(UWorld* InWorld, FActorContainer& InActorContainer, FChunk& InChunk)
: World(InWorld), ActorContainer(InActorContainer), Chunk(InChunk)
{
	// The Reason the goat climbs the mountain is its stubbornness.
}

void FVoxelGenerator::SetVoxelDataInChunk(const FIntVector& VoxelIdx, TSubclassOf<AActor>& BPActor, float Width, float Height)
{
	// VoxelIdx가 Chunk 범위 안에 있는지 체크
	if (!Chunk.IsInsideChunk(VoxelIdx))
	{
		if (DEBUGMODE) 
		{
			FString DebugMessage = FString::Printf(TEXT("FVoxelGenerator::GenerateVoxelIntoChunk {Voxel Index Not Inside Chunk}"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, DebugMessage);
			UE_LOG(LogTemp, Error, TEXT("%s"), *DebugMessage);
		}
		return;
	}

	// 설정할 Voxel 레퍼런스 얻기
	FVoxel& TargetVoxel = Chunk.Chunk[VoxelIdx.X][VoxelIdx.Y][VoxelIdx.Z];

	// Voxel 설정
	TargetVoxel.Transform = GetWorldTransformFromVoxelIndex(VoxelIdx, Width, Height);
	TargetVoxel.BPActor = BPActor;

	// 설정된 Voxel Index List 업데이트
	VoxelIdxsInChunkList.Add(VoxelIdx);
}

void FVoxelGenerator::DeleteVoxelDataInChunk(const FIntVector& VoxelIdx)
{
	// VoxelIdx가 Chunk 범위 안에 있는지 체크
	if (!Chunk.IsInsideChunk(VoxelIdx))
	{
		if (DEBUGMODE) 
		{
			FString DebugMessage = FString::Printf(TEXT("FVoxelGenerator::GenerateVoxelIntoChunk {Voxel Index Not Inside Chunk}"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, DebugMessage);
			UE_LOG(LogTemp, Error, TEXT("%s"), *DebugMessage);
		}
		return;
	}

	// 설정할 Voxel 레퍼런스 얻기
	FVoxel& TargetVoxel = Chunk.Chunk[VoxelIdx.X][VoxelIdx.Y][VoxelIdx.Z];

	// Voxel 설정
	TargetVoxel.Transform = FTransform::Identity;
	TargetVoxel.BPActor = nullptr;

	// 설정된 Voxel Index List 업데이트
	VoxelIdxsInChunkList.Remove(VoxelIdx);
}

void FVoxelGenerator::SpawnActorFromVoxel(FVoxel& Voxel)
{
	if (!Voxel.BPActor)
	{
		if (DEBUGMODE) 
		{
			FString DebugMessage = FString::Printf(TEXT("FVoxelGenerator::GeneratorVoxel {No Voxel.BPActor}"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, DebugMessage);
			UE_LOG(LogTemp, Error, TEXT("%s"), *DebugMessage);
		}
		return;
	}

	if (!World)
	{
		if (DEBUGMODE) 
		{
			FString DebugMessage = FString::Printf(TEXT("FVoxelGenerator::GeneratorVoxel {No World}"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, DebugMessage);
			UE_LOG(LogTemp, Error, TEXT("%s"), *DebugMessage);
		}
		return;
	}

	// 레벨에 복셀 기반 액터 소환
	AActor* NewActor = World -> SpawnActor<AActor> (
		Voxel.BPActor,
		Voxel.Transform.GetLocation(),
		Voxel.Transform.Rotator()
	);

	// Voxel에 저장
	Voxel.SpawnedActor = NewActor;
}

void FVoxelGenerator::DestroyActorFromVoxel(FVoxel& Voxel)
{
	AActor* ActorToDestroy = Voxel.SpawnedActor.Get();

 	if (IsValid(ActorToDestroy))
 	{
 		ActorToDestroy->Destroy();
 	}
    
 	Voxel.SpawnedActor = nullptr;
}
FTransform FVoxelGenerator::GetWorldTransformFromVoxelIndex(const FIntVector& VoxelIdx, float Width, float Height)
{
	float x = VoxelIdx.X;
	float y = VoxelIdx.Y;
	float z = VoxelIdx.Z;

	x *= Width;
	y *= Width;
	z *= Height;

	FVector WorldLocation = FVector(x, y, z);
	return FTransform(FQuat(FRotator(0, 0, 0)),WorldLocation, FVector(1.0F));
}

// ------------------------------------------------- FCreepWayGenerator ------------------------------------------------- //

FCreepWayGenerator::FCreepWayGenerator(UWorld* InWorld, FActorContainer& InActorContainer, FChunk& InChunk)
: FVoxelGenerator(InWorld, InActorContainer, InChunk)
{
	// Visted 초기화
	Visited.SetNum(Chunk.ChunkSize.X);
	for (int32 X = 0; X < Chunk.ChunkSize.X; X++)
	{
		Visited[X].SetNum(Chunk.ChunkSize.Y);
		for(int32 Y = 0; Y < Chunk.ChunkSize.Y; Y++)
		{
			Visited[X][Y].SetNum(Chunk.ChunkSize.Z);
			for (int32 Z = 0; Z < Chunk.ChunkSize.Z; Z++)
			{
				Visited[X][Y][Z] = false;
			}
		}
	}
}

bool FCreepWayGenerator::GenerateCreepWay(int32 ScanRadius)
{
    FIntVector CreepWayStartIdx = Chunk.GetEmptyIdx();
    FIntVector CreepWayEndIdx = Chunk.GetEmptyIdx();
	TSubclassOf<AActor> CreepWayActor = ActorContainer.Container[0];
	SetVoxelDataInChunk(CreepWayStartIdx, CreepWayActor, CreepWayBlockWidth, CreepWayBlockHeight);
	SpawnActorFromVoxel(Chunk.Chunk[CreepWayStartIdx.X][CreepWayStartIdx.Y][CreepWayStartIdx.Z]);

    // 경로를 추적하기 위한 스택
    TArray<FIntVector> PathStack;
    bool bPathFound = false;

    // 이동할 방향 정의
    TArray<FIntVector> Directions = {
        FIntVector( 1,  0,  0),
        FIntVector(-1,  0,  0),
        FIntVector( 0,  1,  0),
        FIntVector( 0, -1,  0)
    };
	TArray<FIntVector> ShuffledDirections = Directions;

    // 시작점을 스택에 추가하고 방문 처리
    PathStack.Push(CreepWayStartIdx);
    Visited[CreepWayStartIdx.X][CreepWayStartIdx.Y][CreepWayStartIdx.Z] = true;

	// 방향 리스트를 섞을 확률
	int32 ShuffleProb = 100;

	// 이전 경로, Neighbor를 계산할 때 쓰인다.
	FIntVector CurrentIdx = FIntVector::ZeroValue;
	FIntVector PreviousDir = Directions[0];

	// 확률 + DFS
    while (PathStack.Num() > 0)
    {
        CurrentIdx = PathStack.Last();

        // 목표 지점(EndIdx)에 도달했는지 확인
        if (CurrentIdx == CreepWayEndIdx)
        {
            bPathFound = true;
            break; // 경로 탐색 성공
        }

		// 매번 새로운 순서로 탐색하기 위해 방향 리스트를 섞음

		if (FMath::RandRange(0, 100) > ShuffleProb)
		{
			ShuffledDirections = Directions;
			ShuffledDirections.Remove(PreviousDir);
			ShuffleProb = 100;
			FIntVector UpOrDownVector = FIntVector(0, 0, FMath::RandRange(-1, 1));
			for (int32 i = 0; i < 3; i++) ShuffledDirections[i] = ShuffledDirections[i] + UpOrDownVector;
			for (int32 i = ShuffledDirections.Num() - 1; i > 0; --i)
			{
				int32 j = FMath::RandRange(0, i);
				ShuffledDirections.Swap(i, j); // Fisher-Yates 셔플
			}
		}
		else ShuffleProb -= 5;

		// TArray<FIntVector> ShuffledDirections = Directions;
		// ShuffledDirections.Remove(PreviousDir);
		// FIntVector UpOrDownVector = FIntVector(0, 0, FMath::RandRange(-1, 1));
		// for (int32 i = 0; i < 3; i++) ShuffledDirections[i] = ShuffledDirections[i] + UpOrDownVector;
		// for (int32 i = ShuffledDirections.Num() - 1; i > 0; --i)
		// {
		// 	int32 j = FMath::RandRange(0, i);
		// 	ShuffledDirections.Swap(i, j); // Fisher-Yates 셔플
		// }

        bool bFoundNextStep = false;
        for (const FIntVector& Dir : ShuffledDirections)
        {
            FIntVector NextIdx = CurrentIdx + Dir;

            // 청크 범위 안인지, 그리고 아직 방문하지 않았는지 확인
			if (!Chunk.IsInsideChunk(NextIdx)) continue;
			if (Visited[NextIdx.X][NextIdx.Y][NextIdx.Z]) continue;
			bool bNeighbor = false;
			for (int32 dx = -1 * ScanRadius; dx <= ScanRadius; ++dx)
        		for (int32 dy = -1 * ScanRadius; dy <= ScanRadius; ++dy)
            		for (int32 dz = -1 * ScanRadius; dz <= ScanRadius; ++dz)
					{
						FIntVector ScanIdx = FIntVector(NextIdx.X + dx, NextIdx.Y + dy, NextIdx.Z + dz);
						if (!Chunk.IsInsideChunk(ScanIdx)) continue;

						bool bScan = false;
						for (int32 i = PathStack.Num() - 1; i >= 0 && i >= PathStack.Num() - 5 * ScanRadius; i--)
							if (ScanIdx == PathStack[i]) 
							{
								bScan = true;
								break;
							}
						if (bScan) continue;

						if (Chunk.Chunk[ScanIdx.X][ScanIdx.Y][ScanIdx.Z].BPActor) 
						{
							bNeighbor = true;
						}
					}
			if (bNeighbor) continue;
			// 다음 경로로 전진
			Visited[NextIdx.X][NextIdx.Y][NextIdx.Z] = true;
			PathStack.Push(NextIdx);
			SetVoxelDataInChunk(NextIdx, CreepWayActor, CreepWayBlockWidth, CreepWayBlockHeight);
			SpawnActorFromVoxel(Chunk.Chunk[NextIdx.X][NextIdx.Y][NextIdx.Z]);
			bFoundNextStep = true;
			PreviousDir = Dir;
			break; // 다음 단계로 넘어감
        }

        // 백트래킹 (막다른 길)
        if (!bFoundNextStep)
        {
            // 6방향 모두 갈 곳이 없으므로 현재 위치를 스택에서 제거 (뒤로 가기)
            FIntVector Deleted = PathStack.Pop();
			DeleteVoxelDataInChunk(Deleted);
			DestroyActorFromVoxel(Chunk.Chunk[Deleted.X][Deleted.Y][Deleted.Z]);
        }
    }

	if (DEBUGMODE && !bPathFound) 
	{
		FString DebugMessage = FString::Printf(TEXT("FCreepGenerator::GeneratorCreepWay {No Path}"));
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, DebugMessage);
		UE_LOG(LogTemp, Error, TEXT("%s"), *DebugMessage);
	}

	return bPathFound;
}