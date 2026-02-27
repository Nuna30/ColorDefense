#include "Rifle.h"
#include "Utils/Utils.h"

ARifle::ARifle()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARifle::BeginPlay()
{
	Super::BeginPlay();
}

void ARifle::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ARifle::LeftClick()
{
	// Get Hit actor.
    FHitResult Hit;
	if (!Utils::GetHit(this, MaxRange, Hit, ECollisionChannel::ECC_GameTraceChannel3)) return;

    // Check if the core is same color.
	ACreepShield* HitCreepShield = Cast<ACreepShield>(Hit.GetActor());
	if (!HitCreepShield) return;
	if (HitCreepShield->CreepShieldColor != CurrentColor) return;

    HitCreepShield->OnHit(Damage);

	// Simple Animation.
	OnShoot();
}

void ARifle::ChangeColor(EColor NewColor)
{
	Super::ChangeColor(NewColor);
    UE_LOG(LogTemp, Warning, TEXT("Rifle color change"));

	// Change the Rifle's Color.
	// UMaterialInstanceDynamic* DynMaterial = ToolMeshComponent->CreateAndSetMaterialInstanceDynamic(4);
	// DynMaterial->SetVectorParameterValue(FName("BaseColor"), Utils::GetLinearColor(NewColor));
}