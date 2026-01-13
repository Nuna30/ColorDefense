#include "PlayerBlock.h"
#include "CreepWayGeneratorManager.h"

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
    APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;

	// Calculate the player's reach.
	FVector Location;
	FRotator Rotation;
	AController* OwnerController = OwnerPawn->GetController();
	if (OwnerController == nullptr) return;
    
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

	if (bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit"));
		APlayerBlock* HitPlayerBlock = Cast<APlayerBlock>(Hit.GetActor());
		if (Placed) // Only the placed block can be destroyed.
		{
			HitPlayerBlock->HandleDestruction();
		}
	}

	DrawDebugLine(GetWorld(), Location, End, FColor::Red, false, 2.0f, 0, 1.0f);
}

void APlayerBlock::RightClick()
{
    APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;

	// Calculate the player's reach.
	FVector Location;
	FRotator Rotation;
	AController* OwnerController = OwnerPawn->GetController();
	if (OwnerController == nullptr) return;
    
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

	if (bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit"));
		APlayerBlock* HitPlayerBlock = Cast<APlayerBlock>(Hit.GetActor());
		if (HitPlayerBlock && !HitPlayerBlock->IsHidden())
		{
			// Make PlayerBlock Visible
			HitPlayerBlock->SetOpacity(1);
			HitPlayerBlock->Placed = true;
			UGameInstance* GameInstance = GetGameInstance();
			UCreepWayGeneratorManager* CreepWayGeneratorManager = GameInstance->GetSubsystem<UCreepWayGeneratorManager>();
			CreepWayGeneratorManager->CreepWayGenerator->SpawnInvisibleNeighboringPlaceables(this->Voxel.Index);
		}
	}


}

void APlayerBlock::ShowPreview(EPlayerState PlayerState)
{
	// Doesn't display the preview for placed blocks.
	if (Placed) return;

    APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;

	// Calculate the player's reach.
	FVector Location;
	FRotator Rotation;
	AController* OwnerController = OwnerPawn->GetController();
	if (OwnerController == nullptr) return;
    
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

	APlayerBlock* HitBlock = (bSuccess) ? Cast<APlayerBlock>(Hit.GetActor()) : nullptr;

	// Only do work if the focus has changed
    if (HitBlock != CurrentFocusedBlock)
    {
        // Re-hide the old block
        if (CurrentFocusedBlock && IsValid(CurrentFocusedBlock))
        {
            CurrentFocusedBlock->SetActorHiddenInGame(true);
        }

        // Show the new block
        if (HitBlock)
        {
            HitBlock->SetActorHiddenInGame(false);
        }

        // Update the reference
        CurrentFocusedBlock = HitBlock;
    }
	
	// If the player switches to other tool while Hitting, the PlayerBlock remains visible.
	// To fix this
	if (HitBlock && PlayerState != EPlayerState::HoldingBlock)
		HitBlock->SetActorHiddenInGame(true);
}

void APlayerBlock::HandleDestruction()
{
	// Only Placed PlayerBlock can be destroyed.
	Destroy();
}

void APlayerBlock::SetOpacity(float Percentage)
{
	UMaterialInterface* BaseMaterial = this->BlockMeshComponent->GetMaterial(0);
	UMaterialInstanceDynamic* DynMaterial = this->BlockMeshComponent->CreateDynamicMaterialInstance(0, BaseMaterial);

	if (DynMaterial)
		DynMaterial->SetScalarParameterValue(FName("Opacity"), Percentage);
}