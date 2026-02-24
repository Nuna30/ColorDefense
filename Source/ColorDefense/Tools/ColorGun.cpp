#include "ColorGun.h"
#include "Utils/Utils.h"

AColorGun::AColorGun()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create Components for the Actor
    DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
    RootComponent = DefaultRoot;

    GunMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
    GunMeshComponent->SetupAttachment(RootComponent);
}

void AColorGun::BeginPlay()
{
	Super::BeginPlay();
}

void AColorGun::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsConnecting)
    {
        UpdateChain();
    }
}

void AColorGun::LeftClick()
{
	bIsConnecting = true;
    ConnectedCreepCores.Empty(); // Clear any previous chain
    UpdateChain(); // Try to catch the first creep immediately
}

void AColorGun::UpdateChain()
{	
	// Get Hit actor.
    FHitResult Hit;
	if (!Utils::GetHit(this, MaxRange, Hit, ECollisionChannel::ECC_GameTraceChannel1)) return;

	// Check if the core is same color.
	ACreepCore* HitCreepCore = Cast<ACreepCore>(Hit.GetActor());
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

void AColorGun::LeftClickReleased()
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

void AColorGun::ChangeGunColor(EColor NewColor)
{
	// Update the Color.
	CurrentColor = NewColor;
	FLinearColor TargetColor = Utils::GetLinearColor(CurrentColor);

	// Change the ColorGun's Color.
	UMaterialInstanceDynamic* DynMaterial = GunMeshComponent->CreateAndSetMaterialInstanceDynamic(4);
	DynMaterial->SetVectorParameterValue(FName("BaseColor"), TargetColor);

	// Play SwapVFX.
	UNiagaraComponent* SpawnedEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		SwapVFX,
		GunMeshComponent->GetComponentLocation(),
		GunMeshComponent->GetComponentRotation(),
		FVector(1.0f),            
		true                      
	);
	SpawnedEffect->SetVariableLinearColor(FName("User.Linear Color"), TargetColor);
}

// --- Combo --- //
void AColorGun::PlayComboSound()
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