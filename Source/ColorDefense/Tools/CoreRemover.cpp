#include "CoreRemover.h"
#include "Utils/Utils.h"

ACoreRemover::ACoreRemover()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACoreRemover::BeginPlay()
{
	Super::BeginPlay();
}

void ACoreRemover::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsConnecting)
    {
        UpdateChain();
    }
}

void ACoreRemover::LeftClick()
{
	bIsConnecting = true;
    ConnectedCreepCores.Empty(); // Clear any previous chain
    UpdateChain(); // Try to catch the first creep immediately

	// Simple Animation.
	OnShoot();
}

void ACoreRemover::UpdateChain()
{	
	// Get Hit actor.
    FHitResult Hit;
	if (!Utils::GetHit(this, MaxRange, Hit, ECollisionChannel::ECC_GameTraceChannel1)) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Nothing hit"));
		return;
	}

	// Check if the core is same color.
	ACreepCore* HitCreepCore = Cast<ACreepCore>(Hit.GetActor());
	if (!HitCreepCore) return;
	if (HitCreepCore->CreepCoreColor != CurrentColor) return;

	// Check if the core is already selected.
	if (ConnectedCreepCores.Contains(HitCreepCore)) return;
	ConnectedCreepCores.Add(HitCreepCore);

	// Creep Highlight
	HitCreepCore->SetHighlighted(true);

	// Update Combo
	ComboCount++;
	PlayComboSound();
	if (AColorDefenseGameState* GS = GetWorld()->GetGameState<AColorDefenseGameState>()) GS->AddCombo(1);
}

void ACoreRemover::LeftClickReleased()
{
	// Get GameState.
	AColorDefenseGameState* GS = GetWorld()->GetGameState<AColorDefenseGameState>();

    // Perform the Chain Kill
    for (ACreepCore* Creep : ConnectedCreepCores)
    {
		// Destroy the chained Creeps.
		Creep->HandleDestruction();

		// Add money.
		GS->AddMoney(1); 
    }

	// Reset variables.
	bIsConnecting = false;
    ConnectedCreepCores.Empty();
	ComboCount = 0;
}

void ACoreRemover::ChangeColor(EColor NewColor)
{
	Super::ChangeColor(NewColor);

	// Change the CoreRemover's Color.
	UMaterialInstanceDynamic* DynMaterial = ToolMeshComponent->CreateAndSetMaterialInstanceDynamic(4);
	DynMaterial->SetVectorParameterValue(FName("BaseColor"), Utils::GetLinearColor(NewColor));
}

// --- Combo --- //
void ACoreRemover::PlayComboSound()
{
	float NewPitch = 1.0f + (ComboCount * PitchMultiplier);

	UGameplayStatics::PlaySoundAtLocation(
		GetWorld(),                   
		ComboSFX,         
		GetActorLocation(),      
		1.0f,                
		NewPitch
	);
}