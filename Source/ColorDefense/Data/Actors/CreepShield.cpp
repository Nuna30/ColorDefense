// Fill out your copyright notice in the Description page of Project Settings.

#include "Utils/Utils.h"
#include "Data/Actors/CreepShield.h"

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

void ACreepShield::ChangeColor(EColor Color)
{
	// Update the member.
    CreepShieldColor = Color;

	// Transform EColor into LinearColor.
	FLinearColor TargetColor = Utils::GetLinearColor(Color);

	// Get first MI and change the color.
	UMaterialInstanceDynamic* DynMaterial = CreepShield->CreateAndSetMaterialInstanceDynamic(0);
	DynMaterial->SetVectorParameterValue(FName("BaseColor"), TargetColor);
}