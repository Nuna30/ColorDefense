#include "Data/ActorComponents/Recoil.h"

// ========================= //
// ===== Base Function ===== //
// ========================= //

URecoil::URecoil()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URecoil::BeginPlay()
{
	Super::BeginPlay();
}

void URecoil::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// DIRECT PLAYERCHARACTER LOOKUP
	if (!OwnerCharacter)
	{
		OwnerCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}

	// Store old values before recovery
	float OldPitch = CurrentRecoilPitch;
	float OldYaw   = CurrentRecoilYaw;

	// Smoothly recover recoil back to zero every frame
	CurrentRecoilPitch = FMath::FInterpTo(CurrentRecoilPitch, 0.0f, DeltaTime, RecoilRecoverySpeed);
	CurrentRecoilYaw   = FMath::FInterpTo(CurrentRecoilYaw,   0.0f, DeltaTime, RecoilRecoverySpeed);

	// Apply the CHANGE (this is what makes the screen kick up on fire and settle back)
	float PitchToApply = OldPitch - CurrentRecoilPitch;
	float YawToApply   = OldYaw   - CurrentRecoilYaw;

	// Kick
	OwnerCharacter->AddControllerPitchInput(-1 * PitchToApply);
	OwnerCharacter->AddControllerYawInput(-1 * YawToApply);
}

// ==================== //
// ===== Features ===== //
// ==================== //

void URecoil::ApplyRecoil()
{
	// Improved kick: vertical has tiny randomness so it feels alive
	float RandomVertical = FMath::RandRange(-0.25f, 0.35f);        // small variation
	CurrentRecoilPitch += VerticalRecoilPerShot + RandomVertical;

	// Horizontal stays the same
	float HorizontalKick = FMath::RandRange(-HorizontalRecoilAmount, HorizontalRecoilAmount);
	CurrentRecoilYaw += HorizontalKick;

	// Prevent infinite climb
	CurrentRecoilPitch = FMath::Clamp(CurrentRecoilPitch, 0.0f, MaxRecoilBuildup);
}

void URecoil::ResetRecoil()
{
	// Instant snap back when you stop firing (feels super responsive)
	CurrentRecoilPitch = -0.5f;
}