#include "PlayerBlock.h"
#include "GeneratorManagers/PlayerBlockGeneratorManager.h"

APlayerBlock::APlayerBlock()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
    RootComponent = DefaultRoot;

    BlockMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh"));
    BlockMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts
void APlayerBlock::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerBlock::LeftClick()
{
	APlayerBlock* HitPlayerBlock = GetHitPlayerBlock();
	if (!HitPlayerBlock) return;
	if (!HitPlayerBlock->Placed) return; // Only the placed block can be destroyed.

	HitPlayerBlock->SetOpacity(0.1);
	HitPlayerBlock->Placed = false;
	HitPlayerBlock->SetActorHiddenInGame(true);
	HitPlayerBlock->SetCollisionWithPawn(false);
}

void APlayerBlock::RightClick()
{
	APlayerBlock* HitPlayerBlock = GetHitPlayerBlock();
	if (!HitPlayerBlock) return;
	if (HitPlayerBlock->Placed) return;

	// Place a PlayerBlock
	HitPlayerBlock->SetOpacity(1);
	HitPlayerBlock->Placed = true;
	HitPlayerBlock->SetActorHiddenInGame(false);
	HitPlayerBlock->SetCollisionWithPawn(true);
	GetPlayerBlockGenerator()->SpawnInvisibleNeighboringPlaceables(HitPlayerBlock->Voxel.Index);
}

void APlayerBlock::ShowPreview(EPlayerState PlayerState)
{
	APlayerBlock* HitPlayerBlock = GetHitPlayerBlock();

	if (!HitPlayerBlock) return; // Check nullptr first.
	if (HitPlayerBlock->Placed) return; // Doesn't display the preview for placed blocks.

	// Only do work if the focus has changed
    if (HitPlayerBlock != CurrentFocusedBlock)
    {
        // Re-hide the old block
		// If the placed block, stay visile
        if (CurrentFocusedBlock && !CurrentFocusedBlock->Placed)
        {
            CurrentFocusedBlock->SetActorHiddenInGame(true);
        }

        // Show the new block
        if (HitPlayerBlock)
        {
            HitPlayerBlock->SetActorHiddenInGame(false);
        }

        // Update the reference
        CurrentFocusedBlock = HitPlayerBlock;
    }
	
	// If the player switches to other tool while Hitting, the PlayerBlock remains visible.
	// To fix this
	if (HitPlayerBlock && PlayerState != EPlayerState::HoldingBlock)
		HitPlayerBlock->SetActorHiddenInGame(true);
}

void APlayerBlock::HandleDestruction()
{
	// Don't need to destroy.
}

void APlayerBlock::SetOpacity(float Percentage)
{
	UMaterialInterface* BaseMaterial = this->BlockMeshComponent->GetMaterial(0);
	UMaterialInstanceDynamic* DynMaterial = this->BlockMeshComponent->CreateDynamicMaterialInstance(0, BaseMaterial);

	if (DynMaterial)
		DynMaterial->SetScalarParameterValue(FName("Opacity"), Percentage);
}

APlayerBlock* APlayerBlock::GetHitPlayerBlock()
{
  	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return nullptr;

	// Calculate the player's reach.
	FVector Location;
	FRotator Rotation;
	AController* OwnerController = OwnerPawn->GetController();
	if (OwnerController == nullptr) return nullptr;
    
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	FVector End = Location + Rotation.Vector() * MaxRange;

	FHitResult Hit;
	FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this); // Ignore the block in your hand
    QueryParams.AddIgnoredActor(GetOwner()); // Ignore the player holding it
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(
        Hit, 
        Location, 
        End, 
        ECollisionChannel::ECC_GameTraceChannel2, 
        QueryParams
    );

	return (bSuccess) ? Cast<APlayerBlock>(Hit.GetActor()) : nullptr;
}

UPlayerBlockGenerator* APlayerBlock::GetPlayerBlockGenerator()
{
	UPlayerBlockGeneratorManager* PlayerBlockGeneratorManager = GetWorld()->GetSubsystem<UPlayerBlockGeneratorManager>();
	return PlayerBlockGeneratorManager->PlayerBlockGenerator;
}

void APlayerBlock::SetCollisionWithPawn(bool bCollision)
{
	// Set collision to "Block" for Pawns (the Player)
	if (bCollision)	BlockMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	else BlockMeshComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}