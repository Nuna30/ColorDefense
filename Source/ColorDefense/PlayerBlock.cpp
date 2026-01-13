#include "PlayerBlock.h"

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
		if (HitPlayerBlock)
		{
			HitPlayerBlock->HandleDestruction();
		}
	}

	DrawDebugLine(GetWorld(), Location, End, FColor::Red, false, 2.0f, 0, 1.0f);
}

void APlayerBlock::RightClick()
{

}

void APlayerBlock::HandleDestruction()
{
	Destroy();
}
