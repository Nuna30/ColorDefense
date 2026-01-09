#include "PlayerBlock.h"

APlayerBlock::APlayerBlock()
{
    BlockMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh"));
    BlockMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts
void APlayerBlock::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerBlock::Use()
{
    
}
