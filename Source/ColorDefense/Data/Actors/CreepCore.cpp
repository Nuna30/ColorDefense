// Fill out your copyright notice in the Description page of Project Settings.

#include "Utils/Utils.h"
#include "Data/Actors/CreepCore.h"

ACreepCore::ACreepCore()
{
	PrimaryActorTick.bCanEverTick = false;

	// SM_CreepCore
    CreepCore = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CreepCore"));
    RootComponent = CreepCore;
}

void ACreepCore::BeginPlay()
{
	Super::BeginPlay();
	
	// Capture the initial size of the CreepCore.
    OriginalScale = CreepCore->GetRelativeScale3D();
}

void ACreepCore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACreepCore::ChangeColor(EColor Color)
{
	// Update the member.
    CreepCoreColor = Color;

	// Transform EColor into LinearColor.
	FLinearColor TargetColor = Utils::GetLinearColor(Color);

	// Get first MI and change the color.
	UMaterialInstanceDynamic* DynMaterial = CreepCore->CreateAndSetMaterialInstanceDynamic(0);
	DynMaterial->SetVectorParameterValue(FName("BaseColor"), TargetColor);
}

void ACreepCore::SetHighlighted(bool bHighlight)
{
    UMaterialInstanceDynamic* DynMaterial = CreepCore->CreateAndSetMaterialInstanceDynamic(0);

    float TargetOpacity = bHighlight ? 0.75f : 1.0f;
    DynMaterial->SetScalarParameterValue(FName("Opacity"), TargetOpacity);

    FVector TargetScale = bHighlight ? (OriginalScale * 1.2f) : OriginalScale;
    CreepCore->SetRelativeScale3D(TargetScale);
}

void ACreepCore::HandleDestruction()
{
	// Spawn Creep Core Destroy VFX.
	UNiagaraComponent* SpawnedEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		CreepCoreDestroyVFX,
		GetActorLocation(),         
		GetActorRotation(),         
		FVector(1.0f),              
		true                                 
	);

	// Change the Destroy VFX Color dynamically.
	FLinearColor TargetColor = Utils::GetLinearColor(this->CreepCoreColor);
	SpawnedEffect->SetVariableLinearColor(FName("User.Linear Color"), TargetColor);

	// Play Destroy SFX.
	UGameplayStatics::PlaySoundAtLocation(
		GetWorld(),                   
		CreepCoreDestroySFX,         
		GetActorLocation(),      
		1.0f,                
		Utils::ColorToPitch(CreepCoreColor) 
	);

	// Destroy CreepCore.
	Destroy();
}
