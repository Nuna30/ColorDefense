#pragma once

#include "InputAction.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"   // For GetCharacterMovement()
#include "Camera/CameraComponent.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Flying.generated.h"

UCLASS()
class COLORDEFENSE_API UFlying : public UActorComponent
{
	GENERATED_BODY()
	UFlying();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:	// --- States --- //

	UPROPERTY() // Whether the player is currently in flying mode 
	bool bIsFlying;

	UPROPERTY() // Time of the last Jump press (for double-tap detection)
	float LastJumpPressTime;

	UPROPERTY()
	float CurrentFlightSpeed;

	UPROPERTY() // Target velocity vector we will smooth toward (for camera-relative movement)
	FVector TargetVelocity;

public: // --- Input Actions --- //

	UPROPERTY(EditAnywhere, Category = "Setting")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Setting")
	UInputAction* CrouchAction;

public: // --- Input Handlers --- //

	UFUNCTION()
	void OnJumpAction(const FInputActionValue& Value);

	UFUNCTION()
	void OnJumpHeld(const FInputActionValue& Value);

	UFUNCTION()
	void OnJumpReleased(const FInputActionValue& Value);

	UFUNCTION()
	void OnCrouchHeld(const FInputActionValue& Value);

	UFUNCTION()
	void OnCrouchReleased(const FInputActionValue& Value);

public: // --- WASD --- //

	float ForwardInput = 0.0f;
	float RightInput = 0.0f;
	bool bIsAscending = false;
	bool bIsDescending = false;
	float DoubleTapThreshold = 0.3f;   // time window for double-tap (Minecraft feel)

	// Physics cache (for clean restore)
	float OriginalGravityScale = 1.0f;
	float OriginalBrakingDecelerationFlying = 0.0f;
	bool  OriginalUseSeparateBrakingFriction = false;

	UFUNCTION()
	void SetForwardInput(float Value);

	UFUNCTION()
	void SetRightInput(float Value);

	UFUNCTION()
	void Fly(float DeltaTime);

public: // --- Input Setup --- //

	void SetupFlyingInput(UEnhancedInputComponent* EnhancedInputComponent);

public: // --- Movement Mode Toggle --- //

	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;

	UFUNCTION()
	void EnterFlyingMode();

	UFUNCTION()
	void ExitFlyingMode();

public: // --- Utils --- //
	class APlayerController* PC;
};
