// Fill out your copyright notice in the Description page of Project Settings.

#include "Utils/Utils.h"
#include "Data/Actors/CreepShield.h"

// ========================= //
// ===== Base Function ===== //
// ========================= //

ACreepShield::ACreepShield()
{
	PrimaryActorTick.bCanEverTick = false;

	// SM_CreepShield
    CreepShield = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CreepShield"));
    RootComponent = CreepShield;
}

void ACreepShield::BeginPlay()
{
	Super::BeginPlay();
}

void ACreepShield::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// ==================== //
// ===== Features ===== //
// ==================== //

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

	// Hit the Shield!
	CreepShieldHP -= Damage;
	if (CreepShieldHP <= 0) HandleDestruction();
}

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