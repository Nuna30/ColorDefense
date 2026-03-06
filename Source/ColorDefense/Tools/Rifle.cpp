#include "Rifle.h"
#include "Utils/Utils.h"

// ========================= //
// ===== Base function ===== //
// ========================= //

ARifle::ARifle()
{
	PrimaryActorTick.bCanEverTick = true;

    RecoilComponent = CreateDefaultSubobject<URecoil>(TEXT("RecoilComponent"));
}

void ARifle::BeginPlay()
{
	Super::BeginPlay();
}

void ARifle::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// =============================== //
// ===== Handle Mouse Input ====== //
// =============================== //

void ARifle::LeftClick()
{
    StartFiring();
}

void ARifle::LeftClickReleased()
{
    StopFiring();
}


// ================= //
// ===== Utils ===== //
// ================= //

void ARifle::StartFiring()
{
    Fire();  // immediate first shot

    GetWorldTimerManager().SetTimer(
        AutoFireTimerHandle,
        this,
        &ARifle::Fire,
        TimeBetweenShots,
        true   // loop
    );
}

void ARifle::StopFiring()
{
    RecoilComponent->ResetRecoil();
    GetWorldTimerManager().ClearTimer(AutoFireTimerHandle);
}

void ARifle::UnEquip()
{
	StopFiring();
	Super::UnEquip();
}

// ==================== //
// ===== Features ===== //
// ==================== //

void ARifle::Fire()
{
    // Animation (blueprint event)
    OnShoot();

    // Fire SFX
    UGameplayStatics::PlaySoundAtLocation(
		GetWorld(),                   
		FireSFX,         
		GetActorLocation(),      
		1.0f,                
		1.0f 
	);

    // Apply Recoil
    RecoilComponent->ApplyRecoil();

	// Hit the shield.
    FHitResult Hit;
    if (!Utils::GetHit(this, MaxRange, Hit, ECollisionChannel::ECC_GameTraceChannel3)) return;

	// Check the color.
    ACreepShield* HitCreepShield = Cast<ACreepShield>(Hit.GetActor());
    if (!HitCreepShield) return;
    if (HitCreepShield->CreepShieldColor != CurrentColor) return;

    HitCreepShield->OnHit(Damage);
}

void ARifle::ChangeColor(EColor NewColor)
{
	Super::ChangeColor(NewColor);
    UE_LOG(LogTemp, Warning, TEXT("Rifle color change"));

	// Change the Rifle's Color.
	// UMaterialInstanceDynamic* DynMaterial = ToolMeshComponent->CreateAndSetMaterialInstanceDynamic(4);
	// DynMaterial->SetVectorParameterValue(FName("BaseColor"), Utils::GetLinearColor(NewColor));
}