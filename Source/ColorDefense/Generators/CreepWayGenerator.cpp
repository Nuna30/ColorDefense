// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/Utils.h"
#include "CreepWayGenerator.h"
#include "CreepRail.h"
#include "GeneratorManagers/PlayerBlockGeneratorManager.h"

#define DEBUGMODE 1

UCreepWayGenerator::UCreepWayGenerator()
{
}

void UCreepWayGenerator::Initialize
(
	UWorld* InWorld, 
	UBPActorPool* InBPActorPool, 
	UVoxelGrid* InVoxelGrid, 
	TArray<UCreepCheckPointGenerator*>& InCreepCheckPointGenerators, 
	int32 InMaxRailCount, 
	int32 InRailLength, 
	TArray<FIntVector> InDirectionContainer,
	float InVoxelWidth, 
	float InVoxelHeight
)
{
	// Initialize variables using parameters.
    Super::Initialize(InWorld, InBPActorPool, InVoxelGrid, InVoxelWidth, InVoxelHeight);
	this->CreepCheckPointGenerators = InCreepCheckPointGenerators;
	this->DirectionContainer = InDirectionContainer;
	
	// Initialize the others.
	this->CurrentDirection = FIntVector(1, 0, 0);
	FIntVector StartIndex = this->VoxelGrid->GetCenterIndex();
	
	this->CreepRail = NewObject<UCreepRail>(this, UCreepRail::StaticClass());
	this->CreepRail->Initialize(InMaxRailCount, InRailLength, StartIndex, this->CurrentDirection);

	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	UPlayerBlockGeneratorManager* PlayerBlockGeneratorManager = GameInstance->GetSubsystem<UPlayerBlockGeneratorManager>();
	this->PlayerBlockGenerator = PlayerBlockGeneratorManager->PlayerBlockGenerator;
}

bool UCreepWayGenerator::DeleteCurrentCreepWay()
{
	if (StepHistoryStack.Num() <= 1) return false;

    // Pop the last history entry.
    FCreepWayStepHistory LastStep = StepHistoryStack.Pop();

	// Restore the previous last indices and the previous direction.
    this->CreepRail->LastIndicesOfEachRail = LastStep.PreviousRailLastIndices;
	this->CurrentDirection = LastStep.PreviousDirection;

	// Update the CreepEnds.
	this->CreepRail->UpdateCreepEnds();

	// Clean up Voxels and Actors.
	int32 DeleteCount = 0;
	for (const FIntVector& Index : LastStep.VoxelIndices)
    {
		// Delete actor.
        FVoxel& Voxel = VoxelGrid->GetVoxel(Index);
		DestroyActorFromVoxel(Voxel);
		DeleteCount++;
    }
	UE_LOG(LogTemp, Warning, TEXT("DeleteCount : %d"), DeleteCount);

	// Delete checkpoint actors.
	for (AActor* CP : LastStep.SpawnedCheckPoints) CP->Destroy();

	// Clean up the locations array in each checkpoint geneator.
	for (int32 i = 0; i < this->CreepCheckPointGenerators.Num(); i++)
    {
		this->CreepCheckPointGenerators[i]->PopLocations(LastStep.CheckPointCountsPerGenerator[i]);
    }

	// Clean up the main buffer
    this->CreepRail->MainBuffer.Empty();

	// Deletion successfully completed.
	return true;
}


void UCreepWayGenerator::GenerateNextCreepWay()
{
	// Prep the history entry.
    FCreepWayStepHistory NewStep;
    NewStep.PreviousRailLastIndices = this->CreepRail->LastIndicesOfEachRail;
    NewStep.PreviousDirection = this->CurrentDirection;
	StepHistoryStack.Push(NewStep);

	// Clean up the buffers in advance.
	CurrentCreepWayIndices.Empty();
	CurrentCheckPoints.Empty();
	this->CreepRail->MainBuffer.Empty();
	CurrentCheckPointCounts.Init(0, this->CreepRail->MaxRailCount);

    // Spawn CheckPoints and Insert CreepWay voxel data.
    this->NextDirection = this->DirectionContainer.Last();
    if (this->NextDirection.Z == 0) GoStraightAndTurnLeftOrRightAndGoStraight();
    else GoStraightAndUpOrDownAndGoStraight();
	SpawnCheckPointsAtLastIndices();

    // Spawn CreepWays.
    FlushRailBuffersToMainBuffer(); 
    SpawnActorWithFlushingMainBuffer(); // Update CurrentSegmentIndices.

    // Update the history entry with results
    StepHistoryStack.Last().VoxelIndices = CurrentCreepWayIndices;
	StepHistoryStack.Last().SpawnedCheckPoints = CurrentCheckPoints;
	StepHistoryStack.Last().CheckPointCountsPerGenerator = CurrentCheckPointCounts;

	// Debugging
	UE_LOG(LogTemp, Warning, TEXT("SpawnCount : %d"), StepHistoryStack.Last().VoxelIndices.Num());
}

void UCreepWayGenerator::GenerateStartLocation() // deprecated.
{
	// Advance the path by two blocks.
	this->CreepRail->InsertCreepWayDataRectangleIntoRailBuffers(this->CurrentDirection, 0, false);
	this->CreepRail->InsertCreepWayDataRectangleIntoRailBuffers(this->CurrentDirection, 0, false);

	// And another one, but patterned.
	GenerateNextCreepWay();
}

void UCreepWayGenerator::GenerateCreepWay()
{
	// 일단 두 칸 가고
	this->CreepRail->InsertCreepWayDataRectangleIntoRailBuffers(this->CurrentDirection, 0, false);
	this->CreepRail->InsertCreepWayDataRectangleIntoRailBuffers(this->CurrentDirection, 0, false);

	// 이후 DirectionContainer를 보고 CreepWay 생성
	// 근데 첫 번째 Direction은 ChunkGenerator에서만 쓰이는 원소였어서 여기서는 무시해야함
	for (int i = 1; i < this->DirectionContainer.Num(); i++)
	{
		this->NextDirection = this->DirectionContainer[i];
		if (this->NextDirection.Z == 0) GoStraightAndTurnLeftOrRightAndGoStraight();
		else GoStraightAndUpOrDownAndGoStraight();
	}

	FlushRailBuffersToMainBuffer();
	SpawnActorWithFlushingMainBuffer();
}

void UCreepWayGenerator::SpawnActorWithFlushingMainBuffer()
{
	// Spawn all CreepWays flushing the main buffer. 
	for (const TTuple<FIntVector, int32, float, EVoxelProperty>& CreepWayData : this->CreepRail->MainBuffer)
	{
		// Spawn the CreepWay.
		SetVoxelDataInVoxelGrid(CreepWayData.Get<0>(), CreepWayData.Get<1>(), CreepWayData.Get<2>(), CreepWayData.Get<3>());
		AActor* CreepWayActor = SpawnActorFromVoxel(this->VoxelGrid->GetVoxel(CreepWayData.Get<0>()));

		// I've decided to add null checks to all actor spawning code.
		if (!CreepWayActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("The CreepWayActor is NULL in UCreepWayGenerator::SpawnActorWithFlushingMainBuffer()."));
			return;
		}

		// Record the CreepWay index.
		this->CurrentCreepWayIndices.Add(CreepWayData.Get<0>());

		// The PlayerBlock might be placed around this CreepWay later.
		this->PlayerBlockGenerator->Add(CreepWayData.Get<0>());
	}

	// Generate the PlayerBlocks.
	this->PlayerBlockGenerator->GeneratePlayerBlock();
}

void UCreepWayGenerator::FlushRailBuffersToMainBuffer()
{
	// Flush rail buffers to main buffer.
    for (int32 i = 0; i < this->CreepRail->RailBuffers.Num(); i++)
    {
        // Use Append without MoveTemp to keep the source data,
        // Empty() them afterward for clarity.
        this->CreepRail->MainBuffer.Append(this->CreepRail->RailBuffers[i]);
        this->CreepRail->RailBuffers[i].Empty(); 
    }
}

void UCreepWayGenerator::GoStraightAndTurnLeftOrRightAndGoStraight()
{
	// 일단 앞으로 직진
	this->CreepRail->InsertCreepWayDataRectangleIntoRailBuffers(this->CurrentDirection, 0, false);
	// 코너 구간을 생성하기 전, 최하단 Rail과 최상단 Rail 중 누가 In/Out인지 설정한다.
	UpdateTopRailIn();
	// 코너 구간 생성
	this->CreepRail->InsertCreepWayDataTriangleIntoRailBuffers(this->bTopRailIn, CurrentDirection, 1);
	SpawnCheckPointsAtLastIndices();
	this->CreepRail->InsertCreepWayDataTriangleIntoRailBuffers(this->bTopRailIn, NextDirection, 0);
	// 다시 직진
	this->CurrentDirection = this->NextDirection;
	this->CreepRail->InsertCreepWayDataRectangleIntoRailBuffers(this->CurrentDirection, 0, false);
}	

void UCreepWayGenerator::GoStraightAndUpOrDownAndGoStraight()
{
	// 앞으로 직진
	this->CreepRail->InsertCreepWayDataRectangleIntoRailBuffers(this->CurrentDirection, 0, false);
	// 위아래 이동 + 경사면 진입 전과 후에 CreepCheckPoint 심기
	this->CurrentDirection.Z = this->NextDirection.Z;
	SpawnCheckPointsAtLastIndices();
	this->CreepRail->InsertCreepWayDataRectangleIntoRailBuffers(this->CurrentDirection, 1, true);
	SpawnCheckPointsAtLastIndices();
	// 위로 향하는 경사면이었으면 다음 평면 블록 설치 시 한칸 더 올려서 설치해야하기 때문에 로직에 LastIndices들을 한칸 올린다.
	if (this->CurrentDirection.Z == 1)
	{
		for (int i = 0; i < this->CreepRail->LastIndicesOfEachRail.Num(); i++)
			this->CreepRail->LastIndicesOfEachRail[i] += FIntVector(0, 0, 1);
	}
	// 평면화 해주고 다시 직진
	this->CurrentDirection.Z = 0;
	this->CreepRail->InsertCreepWayDataRectangleIntoRailBuffers(this->CurrentDirection, 0, false);
	// 현재 방향과 다음 방향이 같으니 갱신 작업은 안 해줘도 됨
}

void UCreepWayGenerator::SpawnCheckPointsAtLastIndices()
{
	// Spawn CheckPoints at last indices.
	for (int32 i = 0; i < this->CreepRail->MaxRailCount; i++)
	{
		// Calculate check point location.
		FIntVector LastVoxelIndex = this->CreepRail->LastIndicesOfEachRail[i];
		LastVoxelIndex += FIntVector(0, 0, 1);
		// Spawn and capture the CheckPoint.
		AActor* NewCP = this->CreepCheckPointGenerators[i]->CreateCreepCheckPointByVoxelIndex(LastVoxelIndex);
		if (!NewCP) 
		{
			UE_LOG(LogTemp, Warning, TEXT("The new creep checkpoint is null in UCreepWayGenerator::SpawnCheckPointsAtLastIndices()"));
			return;
		}
		CurrentCheckPoints.Add(NewCP);

		// Record CheckPoint Count.
		CurrentCheckPointCounts[i]++;
	}
}

void UCreepWayGenerator::UpdateTopRailIn()
{
	int32 c = Utils::CardinalDirections.Find(CurrentDirection);
	int32 n = Utils::CardinalDirections.Find(NextDirection);

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