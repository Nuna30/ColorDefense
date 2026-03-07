#include "PlayerCharacter.h"   // for camera cast, prevent circular dependency
#include "Data/ActorComponents/Flying.h"

// ========================= //
// ===== Base Function ===== //
// ========================= //

UFlying::UFlying()
{
	PrimaryComponentTick.bCanEverTick = true;

	// Refresh all values.
	bIsFlying = false;
	LastJumpPressTime = 0.0f;
	CurrentFlightSpeed = 1200.0f;     // Nice starting value (matches Minecraft creative feel)
	TargetVelocity = FVector::ZeroVector;
	MovementComponent = nullptr;
}


void UFlying::BeginPlay()
{
	Super::BeginPlay();

	// Get PC, MoveComponent
	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	MovementComponent = OwnerCharacter->GetCharacterMovement();
	PC = Cast<APlayerController>(OwnerCharacter->GetController());
		
	// Cache original physics values
	OriginalGravityScale = MovementComponent->GravityScale;
	OriginalBrakingDecelerationFlying = MovementComponent->BrakingDecelerationFlying;
	OriginalUseSeparateBrakingFriction = MovementComponent->bUseSeparateBrakingFriction;
}

void UFlying::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Fly(DeltaTime);
}

// ======================= //
// ===== Input Setup ===== //
// ======================= //

void UFlying::SetupFlyingInput(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (!EnhancedInputComponent) return;

	// Bind Jump (double-tap toggle + hold to ascend)
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started,   this, &UFlying::OnJumpAction);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &UFlying::OnJumpHeld);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &UFlying::OnJumpReleased);

	// Bind Crouch (hold to descend)
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &UFlying::OnCrouchHeld);
	EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &UFlying::OnCrouchReleased);
}

// ========================= //
// ===== INPUT HANDLERS ===== //
// ========================= //

void UFlying::OnJumpAction(const FInputActionValue& Value)
{
	float CurrentTime = GetWorld()->GetTimeSeconds();

	// Double-tap detection (Minecraft style)
	if (CurrentTime - LastJumpPressTime < DoubleTapThreshold)
	{
		if (bIsFlying)
			ExitFlyingMode();
		else
			EnterFlyingMode();
	}
	else
	{
		// First press - record time
		LastJumpPressTime = CurrentTime;
	}
}

void UFlying::OnJumpHeld(const FInputActionValue& Value)
{
	bIsAscending = true;
}

void UFlying::OnJumpReleased(const FInputActionValue& Value)
{
	bIsAscending = false;
}

void UFlying::OnCrouchHeld(const FInputActionValue& Value)
{
	bIsDescending = true;
}

void UFlying::OnCrouchReleased(const FInputActionValue& Value)
{
	bIsDescending = false;
}

// ================================= //
// ===== Movement Mode Toggle ===== //
// ================================= //

void UFlying::EnterFlyingMode()
{
	if (MovementComponent && !bIsFlying)
	{
		MovementComponent->SetMovementMode(MOVE_Flying);
		MovementComponent->MaxFlySpeed = CurrentFlightSpeed;
		MovementComponent->GravityScale = 0.0f;                    // Disable gravity
		MovementComponent->Velocity = FVector::ZeroVector;         // Clean reset (no carry-over momentum)
		MovementComponent->bOrientRotationToMovement = false;      // Prepares camera-relative movement

		// physics for clean restore
		MovementComponent->bUseSeparateBrakingFriction = true;
		MovementComponent->BrakingFriction = 0.0f;
		MovementComponent->BrakingDecelerationFlying = 0.0f;
		MovementComponent->GroundFriction = 0.0f;
		
		bIsFlying = true;
	}
}

void UFlying::ExitFlyingMode()
{
	if (MovementComponent && bIsFlying)
	{
		MovementComponent->SetMovementMode(MOVE_Walking);

		// === RESTORE ORIGINAL PHYSICS ===
		MovementComponent->GravityScale = OriginalGravityScale;
		MovementComponent->bUseSeparateBrakingFriction = OriginalUseSeparateBrakingFriction;
		MovementComponent->BrakingDecelerationFlying = OriginalBrakingDecelerationFlying;
		MovementComponent->GroundFriction = 8.0f;           // UE default
		MovementComponent->bOrientRotationToMovement = true;

		bIsFlying = false;
	}
}

// ================ //
// ===== WASD ===== //
// ================ //

void UFlying::SetForwardInput(float Value) { ForwardInput = Value; }
void UFlying::SetRightInput(float Value)   { RightInput = Value; }

void UFlying::Fly(float DeltaTime)
{
	// CORE CAMERA-RELATIVE HORIZONTAL MOVEMENT (Minecraft style)
	if (bIsFlying && MovementComponent)
	{
		// Horizontal (camera-relative)
		FRotator ControlRot = PC->GetControlRotation();
		FVector CamForward = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::X);
		FVector CamRight   = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);

		FVector DesiredDirection = (CamForward * ForwardInput) + (CamRight * RightInput);
		DesiredDirection.Normalize();

		FVector DesiredVelocity = DesiredDirection * CurrentFlightSpeed;

		TargetVelocity = FMath::VInterpTo(TargetVelocity, DesiredVelocity, DeltaTime, 10.0f);

		// Vertical (Ascend/Descend)
		float VerticalSpeed = 0.0f;
		if (bIsAscending)  VerticalSpeed = CurrentFlightSpeed;
		if (bIsDescending) VerticalSpeed = -CurrentFlightSpeed;

		TargetVelocity.Z = VerticalSpeed;

		// Apply final velocity
		MovementComponent->Velocity = TargetVelocity;
	}
}