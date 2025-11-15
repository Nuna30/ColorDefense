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
	FChunk Chunk = FChunk(UPChunkSize.X, UPChunkSize.Y, UPChunkSize.Z);
	UWorld* World = GetWorld(); // 액터를 스폰하기 위해선 UWorld 객체가 필요하다.
	FCreepWayGenerator CreepWayGenerator = FCreepWayGenerator(World, ActorContainer, Chunk, UPMaxRailCount, UPRailLength);

	// -------------------------------------- 테스트 -------------------------------------------//
	CreepWayGenerator.GenerateCreepWay();
	TeleportPlayerToLocation(FVector(UPChunkSize.X / 2 * 200 + 200, UPChunkSize.Y / 2 * 200, UPChunkSize.Z / 2 * 200 + 5000));
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
	this->Property = EVoxelProperty::Empty;
	this->Index = FIntVector::ZeroValue;
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

void FChunk::ExpandChunk(const FIntVector& VoxelIndex)
{

}

bool FChunk::IsInsideChunk(const FIntVector& VoxelIndex)
{
	int32 x = VoxelIndex.X;
	int32 y = VoxelIndex.Y;
	int32 z = VoxelIndex.Z;

	bool InsideChunk = true;

	if (!(x >= 0 && x < this->ChunkSize.X)) InsideChunk = false;
	if (!(y >= 0 && y < this->ChunkSize.Y)) InsideChunk = false;
	if (!(z >= 0 && z < this->ChunkSize.Z)) InsideChunk = false;

	return InsideChunk;
}

bool FChunk:: IsEmptyIndex(const FIntVector& VoxelIndex)
{
	return this->Chunk[VoxelIndex.X][VoxelIndex.Y][VoxelIndex.Z].Property == EVoxelProperty::Empty;
}

void FChunk::SetRotation(const FIntVector& VoxelIndex, float Rotation)
{
	FRotator NewRotation = FRotator(0, Rotation, 0);
	this->Chunk[VoxelIndex.X][VoxelIndex.Y][VoxelIndex.Z].Transform.SetRotation(NewRotation.Quaternion());
}


// ------------------------------------------------- FVoxelGenerator ---------------------------------------------------- //

FVoxelGenerator::FVoxelGenerator(UWorld* InWorld, FActorContainer& InActorContainer, FChunk& InChunk)
: World(InWorld), ActorContainer(InActorContainer), Chunk(InChunk)
{
	// The Reason the goat climbs the mountain is its stubbornness.
}

void FVoxelGenerator::SetVoxelDataInChunk(const FIntVector& VoxelIndex, int32 ActorContainerIndex, EVoxelProperty Property)
{
	// VoxelIndex가 Chunk 범위 안에 있는지 체크
	if (!Chunk.IsInsideChunk(VoxelIndex))
	{
		if (DEBUGMODE) 
		{
			FString DebugMessage = FString::Printf(TEXT("FVoxelGenerator::SetVoxelDataInChunk {Voxel Index Not Inside Chunk}"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, DebugMessage);
			UE_LOG(LogTemp, Error, TEXT("%s"), *DebugMessage);
		}
		return;
	}

	if (ActorContainerIndex > ActorContainer.Container.Num() - 1)
	{
		if (DEBUGMODE) 
		{
			FString DebugMessage = FString::Printf(TEXT("FVoxelGenerator::SetVoxelDataInChunk {ActorContainer out of index}"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, DebugMessage);
			UE_LOG(LogTemp, Error, TEXT("%s"), *DebugMessage);
		}
		return;
	}

	// 설정할 Voxel 레퍼런스 얻기
	FVoxel& TargetVoxel = Chunk.Chunk[VoxelIndex.X][VoxelIndex.Y][VoxelIndex.Z];

	// Voxel 설정
	TargetVoxel.Transform = GetWorldTransformFromVoxelIndex(VoxelIndex, VoxelWidth, VoxelHeight);
	TargetVoxel.BPActor = ActorContainer.Container[ActorContainerIndex];
	TargetVoxel.Property = Property;
	TargetVoxel.Index = VoxelIndex;
}

void FVoxelGenerator::DeleteVoxelDataInChunk(const FIntVector& VoxelIndex)
{
	// VoxelIndex가 Chunk 범위 안에 있는지 체크
	if (!Chunk.IsInsideChunk(VoxelIndex))
	{
		if (DEBUGMODE) 
		{
			FString DebugMessage = FString::Printf(TEXT("FVoxelGenerator::DeleteVoxelDataInChunk {Voxel Index Not Inside Chunk}"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, DebugMessage);
			UE_LOG(LogTemp, Error, TEXT("%s"), *DebugMessage);
		}
		return;
	}

	// 설정할 Voxel 레퍼런스 얻기
	FVoxel& TargetVoxel = Chunk.Chunk[VoxelIndex.X][VoxelIndex.Y][VoxelIndex.Z];

	// Voxel 설정
	TargetVoxel.Transform = FTransform::Identity;
	TargetVoxel.BPActor = nullptr;
	TargetVoxel.Property = EVoxelProperty::Empty;
	TargetVoxel.Index = FIntVector::ZeroValue;
}

void FVoxelGenerator::SpawnActorFromVoxel(FVoxel& Voxel)
{
	if (!Voxel.BPActor)
	{
		if (DEBUGMODE) 
		{
			FString DebugMessage = FString::Printf(TEXT("FVoxelGenerator::SpawnActorFromVoxel {No Voxel.BPActor}"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, DebugMessage);
			UE_LOG(LogTemp, Error, TEXT("%s"), *DebugMessage);
		}
		return;
	}

	if (!World)
	{
		if (DEBUGMODE) 
		{
			FString DebugMessage = FString::Printf(TEXT("FVoxelGenerator::SpawnActorFromVoxel {No World}"));
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

FTransform FVoxelGenerator::GetWorldTransformFromVoxelIndex(const FIntVector& VoxelIndex, float Width, float Height)
{
	float x = VoxelIndex.X;
	float y = VoxelIndex.Y;
	float z = VoxelIndex.Z;

	x *= Width;
	y *= Width;
	z *= Height;

	FVector WorldLocation = FVector(x, y, z);
	return FTransform(FQuat(FRotator(0, 0, 0)),WorldLocation, FVector(1.0F));
}

// ---------------------------------------------------------------------------------------------------------------------- //
// ---------------------------------------------------------------------------------------------------------------------- //
// ------------------------------------------------- FCreepWayGenerator ------------------------------------------------- //
// ---------------------------------------------------------------------------------------------------------------------- //
// ---------------------------------------------------------------------------------------------------------------------- //

FCreepWayGenerator::FCreepWayGenerator(UWorld* InWorld, FActorContainer& InActorContainer, FChunk& InChunk, int32 InMaxRailCount, int32 InRailLength)
: FVoxelGenerator(InWorld, InActorContainer, InChunk)
{
	// 최대 레일 개수
	this->MaxRailCount = InMaxRailCount;
	// 레일 길이
	this->RailLength = InRailLength;
	// ODirectionArray은 사방향 벡터를 저장한다.
	this->ODirectionArray = {
		FIntVector(1, 0, 0), FIntVector(-1, 0, 0), FIntVector(0, 1, 0), FIntVector(0, -1, 0)
	};
	// RailBuffers는 각 레일에 해당하는 버퍼들을 저장한다.
	this->RailBuffers.SetNum(MaxRailCount);
	// CurrentDirection은 현재 경로 진행 방향을 저장한다.
	this->CurrentDirection = FIntVector(1, 0, 0);
	// LastIndexesOfEachRail은 각 레일의 마지막 CreepWayBlock의 인덱스를 저장한다.
	FIntVector StartIndex = FIntVector(this->Chunk.ChunkSize.X / 2, this->Chunk.ChunkSize.Y / 2, this->Chunk.ChunkSize.Z / 2);
	for (int32 i = 0; i < this->MaxRailCount; i++)
	{
		this->LastIndexesOfEachRail.Add(StartIndex);
		StartIndex = StartIndex + GetPerpendicularDirection(this->CurrentDirection);
	}
}

void FCreepWayGenerator::GenerateCreepWay()
{
	// 맨 처음 영역 생성
	// InitializeCreepWay();

	for (int32 i = 0; i < 10; i++)
	{
		DecideNextDirection();
		PrintDirections();
		if (this->CurrentDirection == this->NextDirection) GoStraightAndUpOrDownAndGoStraight();
		else GoStraightAndTurnLeftOrRightAndGoStraight();
		this->CurrentDirection = this->NextDirection;
	}

	FlushRailBuffersToMainBuffer();
	SpawnActorWithFlushingMainBuffer(); // 나중에는 이 부분을 크립 삭제 마다 버퍼 하나 비우는 식으로 구현해야 한다.
}

void FCreepWayGenerator::SpawnActorWithFlushingMainBuffer()
{
	for (const FIntVector& VoxelIndex : MainBuffer)
	{
		FIntVector TargetVoxelIndex = VoxelIndex;
		SpawnActorFromVoxel(this->Chunk.Chunk[TargetVoxelIndex.X][TargetVoxelIndex.Y][TargetVoxelIndex.Z]);
	}
}

void FCreepWayGenerator::FlushRailBuffersToMainBuffer()
{
	for (TArray<FIntVector>& RailBuffer : RailBuffers)
	{
		MainBuffer.Append(MoveTemp(RailBuffer));
	}
}

void FCreepWayGenerator::InitializeCreepWay()
{
	if (this->CurrentDirection == this->NextDirection) GoStraightAndUpOrDownAndGoStraight();
	else GoStraightAndTurnLeftOrRightAndGoStraight();

	// 나머지 세팅 (CreepStart, CreepEnd 설치, ...)
}

void FCreepWayGenerator::GoStraightAndTurnLeftOrRightAndGoStraight()
{
	// 일단 앞으로 직진
	LoadVoxelIndexRectangleIntoRailBuffers(0, false);
	// 코너 구간을 생성하기 전, 최하단 Rail과 최상단 Rail 중 누가 In/Out인지 설정한다.
	UpdateTopRailIn();
	// 코너 구간 생성
	LoadVoxelIndexTriangleIntoRailBuffers(CurrentDirection);
	SetLastIndexesOfEachRailToCreepCheckPoint();
	LoadVoxelIndexTriangleIntoRailBuffers(NextDirection);
	// 다시 직진
	this->CurrentDirection = this->NextDirection;
	LoadVoxelIndexRectangleIntoRailBuffers(0, false);
}	

void FCreepWayGenerator::GoStraightAndUpOrDownAndGoStraight()
{
	// 앞으로 직진
	LoadVoxelIndexRectangleIntoRailBuffers(0, false);
	UpdateLastIndexesOfEachRail();
	// 위아래 정하기
	TArray<int32> UpOrDown = {-1, 1};
	this->CurrentDirection.Z = UpOrDown[FMath::RandRange(0, 1)];
	// 위아래 직진
	LoadVoxelIndexRectangleIntoRailBuffers(1, true);
	UpdateLastIndexesOfEachRail();
	// 평면화 해주고 다시 직진
	this->CurrentDirection.Z = 0;
	LoadVoxelIndexRectangleIntoRailBuffers(0, false);
	UpdateLastIndexesOfEachRail();
	// 현재 방향과 다음 방향이 같으니 갱신 작업은 안 해줘도 됨
}

void FCreepWayGenerator::SetLastIndexesOfEachRailToCreepCheckPoint()
{
	// 각 레일의 마지막 인덱스들에 해당하는 복셀의 속성을 CreepCheckPoint로 설정
	for (int32 i = 0; i < this->MaxRailCount; i++)
	{
		FIntVector LastVoxelIndex = this->LastIndexesOfEachRail[i];
		this->Chunk.Chunk[LastVoxelIndex.X][LastVoxelIndex.Y][LastVoxelIndex.Z].Property = EVoxelProperty::CreepCheckPoint;
	}
}

void FCreepWayGenerator::LoadVoxelIndexTriangleIntoRailBuffers(const FIntVector& Direction)
{
	// bTopRailIn 변수를 고려해서 CurrentDirection 방향으로 삼각형 영역의 VoxelIndex를 버퍼에 로딩
	for (int32 i = 0; i < MaxRailCount; i++)
	{
		FIntVector VoxelIndexForRailFromBottom = this->LastIndexesOfEachRail[i];
		TArray<FIntVector>& RailBufferFromBottom = this->RailBuffers[i];
		FIntVector VoxelIndexForRailFromTop = this->LastIndexesOfEachRail[MaxRailCount - i - 1];
		TArray<FIntVector>& RailBufferFromTop = this->RailBuffers[MaxRailCount - i - 1];
		for (int32 j = 0; j < i; j++)
		{
			// TopRail이 In 상태이면 TopRailBuffer부터 처리해줘야한다.
			if (bTopRailIn)
			{
				// 다음 칸으로 이동 (이 코드가 맨 앞으로 와야 LastIndex 앞에 설치함)
				VoxelIndexForRailFromTop = VoxelIndexForRailFromTop + Direction;
				// 인덱스가 청크 안에 존재하면 RailBuffer에 넣고 Chunk에 VoxelData를 생성한다.
				if (!this->Chunk.IsInsideChunk(VoxelIndexForRailFromTop))
					this->Chunk.ExpandChunk(VoxelIndexForRailFromTop);
				RailBufferFromTop.Add(VoxelIndexForRailFromTop);
				this->LastIndexesOfEachRail[MaxRailCount - i - 1] = VoxelIndexForRailFromTop;
				this->SetVoxelDataInChunk(VoxelIndexForRailFromTop, 0, EVoxelProperty::NormalCreepWay);
			}
			else
			{
				// 다음 칸으로 이동 (이 코드가 맨 앞으로 와야 LastIndex 앞에 설치함)
				VoxelIndexForRailFromBottom = VoxelIndexForRailFromBottom + Direction;
				// 인덱스가 청크 안에 존재하면 RailBuffer에 넣고 Chunk에 VoxelData를 생성한다.
				if (!this->Chunk.IsInsideChunk(VoxelIndexForRailFromBottom))
					this->Chunk.ExpandChunk(VoxelIndexForRailFromBottom);
				this->LastIndexesOfEachRail[i] = VoxelIndexForRailFromBottom;
				RailBufferFromBottom.Add(VoxelIndexForRailFromBottom);
				this->SetVoxelDataInChunk(VoxelIndexForRailFromBottom, 0, EVoxelProperty::NormalCreepWay);
			}
		}
	}
}

void FCreepWayGenerator::LoadVoxelIndexRectangleIntoRailBuffers(int32 ActorContainerIndex, bool bRotate)
{
	// 오르막길이면 높이 하나를 낮춰줘야 한다.
	FIntVector DownOffset = FIntVector(0, 0, 0);
	if (this->CurrentDirection.Z == 1) DownOffset.Z = -1;
	// CurrentDirection 방향으로 직사각형 영역의 VoxelIndex를 버퍼에 로딩
	for (int32 i = 0; i < this->MaxRailCount; i++)
	{
		FIntVector VoxelIndexForRail = this->LastIndexesOfEachRail[i] + DownOffset;
		TArray<FIntVector>& RailBuffer = this->RailBuffers[i];					
		for (int32 j = 0; j < this->RailLength; j++)
		{
			// 다음 칸으로 이동 (이 코드가 맨 앞으로 와야 LastIndex 앞에 설치함)
			VoxelIndexForRail = VoxelIndexForRail + this->CurrentDirection;

			// 인덱스가 청크 안에 존재하면 RailBuffer에 넣고 Chunk에 VoxelData를 생성한다.
			if (this->Chunk.IsInsideChunk(VoxelIndexForRail))
			{
				RailBuffer.Add(VoxelIndexForRail);
				this->LastIndexesOfEachRail[i] = VoxelIndexForRail;
				SetVoxelDataInChunk(VoxelIndexForRail, ActorContainerIndex, EVoxelProperty::NormalCreepWay);
				// Up/Down 유무와 방향에 맞게 SlopeCreepWayBlock을 회전시킨다.
				if (bRotate) RotateSlopeCreepWayBlock(VoxelIndexForRail);
			}
			else if (DEBUGMODE)
			{
				FString DebugMessage = FString::Printf(TEXT("FCreepWayGenerator::LoadVoxelIndexRectangleIntoRailBuffers {VoxelIndexForRail is not inside chunk!}"));
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, DebugMessage);
				UE_LOG(LogTemp, Error, TEXT("%s"), *DebugMessage);					
				return;
			}
		}
	}
	// 오르막길이면 마지막에 한 칸 높여서 평면 블록을 설치해줘야 한다.
	if (this->CurrentDirection.Z == 1)
	{
		for (int32 i = 0; i < this->MaxRailCount; i++)
		{
			FIntVector VoxelIndexForRail = this->LastIndexesOfEachRail[i] + this->CurrentDirection;
			this->RailBuffers[i].Add(VoxelIndexForRail);
			this->LastIndexesOfEachRail[i] = VoxelIndexForRail;
			// Slope를 쓰다가 이 부분만 평면을 사용한다. 그래서 그냥 하드코딩 했다. 위험 부분.
			SetVoxelDataInChunk(VoxelIndexForRail, 0, EVoxelProperty::NormalCreepWay);
		}
	}
}

void FCreepWayGenerator::UpdateLastIndexesOfEachRail()
{
	// LastIndexesOfEachRail 갱신
	for (int32 i = 0; i < this->MaxRailCount; i++)
	{
		this->LastIndexesOfEachRail[i] = RailBuffers[i].Last();
	}
}

FIntVector FCreepWayGenerator::GetPerpendicularDirection(const FIntVector& Direction)
{
	if (Direction.X == 1) return FIntVector(0, 1, Direction.Z);
	else if (Direction.Y == 1) return FIntVector(1, 0, Direction.X);
	else return FIntVector::ZeroValue;
}

void FCreepWayGenerator::DecideNextDirection()
{
	// 방향배열 원본 복사
	TArray<FIntVector> DirectionArray = ODirectionArray;
	// 방향벡터 평면화
	NextDirection.Z = 0;
	// 반대 방향 삭제
	DirectionArray.Remove(CurrentDirection * -1);
	// 방향배열에 무작위성 추가 (Fisher-Yates 셔플)
	for (int32 i = 0; i < DirectionArray.Num(); i++)
	{
		int32 j = FMath::RandRange(0, i);		
		DirectionArray.Swap(i, j);
	}

	// 경로 생성해도 되는 방향 선택
	for (int32 i = 0; i < DirectionArray.Num(); i++)
	{
		// CreepWay가 비어있는 방향인지 확인
		FIntVector MaybeNextDirection = DirectionArray[i];
		FIntVector CenterRailLastIndex = this->LastIndexesOfEachRail[this->MaxRailCount / 2];
		// 체크하기 전 우선 현재 CreepWay를 벗어나고
		CenterRailLastIndex += FIntVector(
			MaybeNextDirection.X * this->RailLength, MaybeNextDirection.Y * this->RailLength, MaybeNextDirection.Z * this->RailLength
		);
		// 범위 내를 전부 체크
		bool bKeepGoing = true;
		for (int32 j = this->RailLength / 2 + 1; j < this->RailLength * 2; j++)
		{
			CenterRailLastIndex = CenterRailLastIndex + MaybeNextDirection;
			if (!this->Chunk.IsEmptyIndex(CenterRailLastIndex)) // 이거 나중에 사용자 블록은 제외시켜야 할 듯?
			{
				bKeepGoing = false;
				break;
			}
		}
		// 비어있으면 NextDirection 결정, 함수 종료
		if (bKeepGoing) 
		{
			print(FString::Printf(TEXT("MaybeNextDirection : %d %d %d"), MaybeNextDirection.X, MaybeNextDirection.Y, MaybeNextDirection.Z));
			this->NextDirection = MaybeNextDirection;
			return;
		}
	}

	// 경로 못 찾을 시 
	// 미구현
}

void FCreepWayGenerator::RotateSlopeCreepWayBlock(const FIntVector& VoxelIndex)
{
	if (this->CurrentDirection == FIntVector(1, 0, 1)) this->Chunk.SetRotation(VoxelIndex, 270);
	else if (this->CurrentDirection == FIntVector(-1, 0, 1)) this->Chunk.SetRotation(VoxelIndex, 90);
	else if (this->CurrentDirection == FIntVector(0, 1, 1)) this->Chunk.SetRotation(VoxelIndex, 0);
	else if (this->CurrentDirection == FIntVector(0, -1, 1)) this->Chunk.SetRotation(VoxelIndex, 180);
	else if (this->CurrentDirection == FIntVector(1, 0, -1)) this->Chunk.SetRotation(VoxelIndex, 90);
	else if (this->CurrentDirection == FIntVector(-1, 0, -1)) this->Chunk.SetRotation(VoxelIndex, 270);
	else if (this->CurrentDirection == FIntVector(0, 1, -1)) this->Chunk.SetRotation(VoxelIndex, 180);
	else if (this->CurrentDirection == FIntVector(0, -1, -1)) this->Chunk.SetRotation(VoxelIndex, 0);
}

void FCreepWayGenerator::UpdateTopRailIn()
{
	int32 c = this->ODirectionArray.Find(CurrentDirection);
	int32 n = this->ODirectionArray.Find(NextDirection);

	if (c == 0)
	{
		if (n == 2) this->bTopRailIn = true;
		else this->bTopRailIn = false;
	}
	else if (c == 1)
	{
		if (n == 2) this->bTopRailIn = false;
		else this->bTopRailIn = true;
	}
	else if (c == 2)
	{
		if (n == 0) this->bTopRailIn = false;
		else this->bTopRailIn = true;
	}
	else
	{
		if (n == 0) this->bTopRailIn = true;
		else this->bTopRailIn = false;
	}
}

void FCreepWayGenerator::PrintLastIndexes()
{
	for (int32 i = 0; i < this->LastIndexesOfEachRail.Num(); i++)
	{
		print(FString::Printf(TEXT("LastIndexesOfEachRail[%d] : %d %d %d"), i, LastIndexesOfEachRail[i].X, LastIndexesOfEachRail[i].Y, LastIndexesOfEachRail[i].Z));		
	}
}

void FCreepWayGenerator::PrintRailBuffers()
{
	for (int32 i = 0; i < RailBuffers.Num(); i++)
	{
		for (int32 j = 0; j < RailBuffers[i].Num(); j++)
		{
			print(FString::Printf(TEXT("RailBuffers[%d][%d] : %d %d %d"), i, j, RailBuffers[i][j].X, RailBuffers[i][j].Y, RailBuffers[i][j].Z));
		}
	}
}

void FCreepWayGenerator::PrintDirections()
{
	print(FString::Printf(TEXT("Current Direction, NextDirection : (%d %d %d), (%d %d %d)"),
	 CurrentDirection.X, CurrentDirection.Y, CurrentDirection.Z, NextDirection.X, NextDirection.Y, NextDirection.Z));
}

// ---------------------------------------------------------------------------------------------------------------------- //
// ---------------------------------------------------------------------------------------------------------------------- //
// --------------------------------------------- CreepCheckPointGenerator ----------------------------------------------- //
// ---------------------------------------------------------------------------------------------------------------------- //
// ---------------------------------------------------------------------------------------------------------------------- //

FCreepCheckPointGenerator::FCreepCheckPointGenerator(FChunk& InChunk)
: Chunk(InChunk)
{

}

void FCreepCheckPointGenerator::GenerateCreepCheckPoint()
{

}

// ---------------------------------------------------------------------------------------------------------------------- //
// ---------------------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------- ETC --------------------------------------------------------------- //
// ---------------------------------------------------------------------------------------------------------------------- //
// ---------------------------------------------------------------------------------------------------------------------- //

void print(FString DebugMessage)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, DebugMessage);
	UE_LOG(LogTemp, Error, TEXT("%s"), *DebugMessage);		
}
