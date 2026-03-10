#include "Generators/ChunkGenerator.h"             
#include "GeneratorManagers/ChunkGeneratorManager.h"

#include "Data/Actors/ChunkVisualizer.h"

// ========================== //
// ===== Base Functions ===== //
// ========================== //

AChunkVisualizer::AChunkVisualizer()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AChunkVisualizer::BeginPlay()
{
	Super::BeginPlay();

	// Wire the ChunkGenerator.
	UChunkGeneratorManager* ChunkGeneratorManager = GetWorld()->GetSubsystem<UChunkGeneratorManager>();
	ChunkGenerator = ChunkGeneratorManager->ChunkGenerator;

	// Bind the delegate.
	ChunkGenerator->OnChunkGenerated.AddDynamic(this, &AChunkVisualizer::OnChunkGenerated);
	ChunkGenerator->OnChunkDeleted.AddDynamic(this, &AChunkVisualizer::OnChunkDeleted);

	// Load the  VisualBlock.
	VisualBlockPath = "/Game/Blueprints/Actors/BP_Grass_Plain.BP_Grass_Plain_C";
	VisualBlock = LoadClass<AActor>(nullptr, *VisualBlockPath);

	// start visualizing
	VisualizeStartingPoint();
}

void AChunkVisualizer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// ========================= //
// ===== Bind Delegate ===== //
// ========================= //

void AChunkVisualizer::OnChunkGenerated()
{
	GenerateVisualBlocks();
}

void AChunkVisualizer::OnChunkDeleted()
{
	DeleteVisualBlocks();
}

// ========================= //
// ===== Visualization ===== //
// ========================= //

void AChunkVisualizer::VisualizeStartingPoint()
{
	for (int i = 0; i < 5; i++)
	{
		FIntVector TmpIdx = ChunkGenerator->ChunkIndexContainer[ChunkGenerator->ChunkIndexContainer.Num() - i - 1];

		AActor* NewVisualBlock = GetWorld()->SpawnActor<AActor> (
		VisualBlock,
		FVector(TmpIdx.X * VisualBlockWidth, TmpIdx.Y * VisualBlockWidth, TmpIdx.Z * VisualBlockHeight),
		FRotator::ZeroRotator
		);

		VisualBlockContainer.Add(NewVisualBlock);
	}
}

void AChunkVisualizer::GenerateVisualBlocks()
{
	for (int i = 0; i < 3; i++)
	{
		FIntVector TmpIdx = ChunkGenerator->ChunkIndexContainer[ChunkGenerator->ChunkIndexContainer.Num() - i - 1];

		AActor* NewVisualBlock = GetWorld()->SpawnActor<AActor> (
		VisualBlock,
		FVector(TmpIdx.X * VisualBlockWidth, TmpIdx.Y * VisualBlockWidth, TmpIdx.Z * VisualBlockHeight),
		FRotator::ZeroRotator
		);

		VisualBlockContainer.Add(NewVisualBlock);
	}
}

void AChunkVisualizer::DeleteVisualBlocks()
{
	for (int i = 0; i < 3; i++)
	{
		AActor* BlockToBeDeleted = VisualBlockContainer.Pop();
		BlockToBeDeleted->Destroy();
	}
}

// ================= //
// ===== Utils ===== //
// ================= //

void AChunkVisualizer::SetStartingLocation(FIntVector StartingLocation)
{
	SpawnOffset = StartingLocation;
}