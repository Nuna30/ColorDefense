// Fill out your copyright notice in the Description page of Project Settings.

#include "Utils/Utils.h"
#include "Data/Actors/CreepShield.h"

// ========================= //
// ===== Base Function ===== //
// ========================= //

ACreepShield::ACreepShield()
{
	PrimaryActorTick.bCanEverTick = false;

	// SKM_CreepShield
    CreepShield = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CreepShield"));
    RootComponent = CreepShield;

	// HP BAR COMPONENT
    HPBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarComponent"));
    HPBarComponent->SetupAttachment(RootComponent);
    HPBarComponent->SetWidgetSpace(EWidgetSpace::Screen);        // Always stares at camera.
	HPBarComponent->SetWidgetClass(UHPBarWidget::StaticClass()); 
}

void ACreepShield::BeginPlay()
{
	Super::BeginPlay();

	// Initialize HP Bar safely.
	GetWorldTimerManager().SetTimerForNextTick(this, &ACreepShield::InitializeHPBarWidget);
}

void ACreepShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// ==================== //
// ===== Features ===== //
// ==================== //

// --- Hit --- //

void ACreepShield::OnHit(int32 Damage)
{
	// Spawn Creep Shield Hit VFX.
	UNiagaraComponent* SpawnedEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		CreepShieldHitVFX,
		GetActorLocation(),         
		GetActorRotation(),         
		FVector(1.0f),              
		true                                 
	);

	// Change the Hit VFX Color dynamically.
	FLinearColor TargetColor = Utils::GetLinearColor(this->CreepShieldColor);
	SpawnedEffect->SetVariableLinearColor(FName("User.Linear Color"), TargetColor);

	// Play Hit SFX.
	UGameplayStatics::PlaySoundAtLocation(
		GetWorld(),                   
		CreepShieldHitSFX,         
		GetActorLocation(),      
		1.0f,                
		Utils::ColorToPitch(CreepShieldColor) 
	);

	// Player Hit Animation.
	PlayHitAnim();
	
	// Hit the Shield!
	CurrentShieldHP -= Damage;

	// Update HP Bar.
	HPBarWidgetInstance->UpdateHealth(CurrentShieldHP, 10);

	// Destroy if HP zero.
	if (CurrentShieldHP <= 0) HandleDestruction();
}

void ACreepShield::PlayHitAnim()
{
    UAnimInstance* AnimInstance = CreepShield->GetAnimInstance();
	CreepShield->PlayAnimation(CreepShieldHitAnim, false); // false = play once, no loop
}

// --- Destroy --- //

void ACreepShield::HandleDestruction()
{
	// Spawn Creep Shield Destroy VFX.
	UNiagaraComponent* SpawnedEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		CreepShieldDestroyVFX,
		GetActorLocation(),         
		GetActorRotation(),         
		FVector(1.0f),              
		true                                 
	);

	// Change the Destroy VFX Color dynamically.
	FLinearColor TargetColor = Utils::GetLinearColor(this->CreepShieldColor);
	SpawnedEffect->SetVariableLinearColor(FName("User.Linear Color"), TargetColor);

	// Play Destroy SFX.
	UGameplayStatics::PlaySoundAtLocation(
		GetWorld(),                   
		CreepShieldDestroySFX,         
		GetActorLocation(),      
		1.0f,                
		Utils::ColorToPitch(CreepShieldColor) 
	);

	Destroy();
}

// ================= //
// ===== Utils ===== //
// ================= //
void ACreepShield::InitializeHPBarWidget()
{
    HPBarComponent->SetDrawSize(FVector2D(280.f, 36.f));
    HPBarComponent->SetRelativeLocation(FVector(0.f, 0.f, 2.f)); // 120cm above Shield
    HPBarComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HPBarWidgetInstance = Cast<UHPBarWidget>(HPBarComponent->GetUserWidgetObject());
	HPBarWidgetInstance->UpdateHealth(CurrentShieldHP, MaxHP);  
	FLinearColor BarColor = Utils::GetLinearColor(CreepShieldColor);
	HPBarWidgetInstance->SetBarColor(BarColor);
}


void ACreepShield::ChangeColor(EColor Color)
{
	// Update the member.
    CreepShieldColor = Color;

	// Transform EColor into LinearColor.
	FLinearColor TargetColor = Utils::GetLinearColor(Color);

	// Get first MI and change the color.
	if (!CreepShield) return;
	UMaterialInstanceDynamic* DynMaterial = CreepShield->CreateAndSetMaterialInstanceDynamic(0);
	if (!DynMaterial) return;
	DynMaterial->SetVectorParameterValue(FName("BaseColor"), TargetColor);
}

void ACreepShield::SetOwnerCreep(ACreep* OwnerCreep)
{
	Owner = OwnerCreep;
}