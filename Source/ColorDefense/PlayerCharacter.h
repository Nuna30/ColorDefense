#pragma once

// --- Input System --- //
#include "InputActionValue.h" // Adding Enhance Input System.
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// --- Components --- //
#include "Camera/CameraComponent.h" // Adding the camera because the rest of the hierarchy is defined in C++.

// --- ETC --- //
#include "Utils/GameEnums.h"

// --- Default --- //
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class COLORDEFENSE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
    APlayerCharacter();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

public: // --- Components --- //
    UPROPERTY(EditAnywhere, Category = "Setting")
    UCameraComponent* FPSCamera;

    UPROPERTY(EditAnywhere, Category = "Setting")
    USceneComponent* InvisibleArm;

    UPROPERTY(EditAnywhere, Category = "Setting")
    UChildActorComponent* ToolBoxComponent;

public: // --- Action --- //
    /** MappingContext for player input */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputMappingContext* DefaultMappingContext;

public: // --- Shopping --- //
    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputAction* ToggleShopAction;

    void ToggleShop();

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// 키 바인딩 wasd 마우스
	void MoveForward(float AxisValue);
	void LookUp(float AxisValue);
	void MoveRight(float AxisValue);
	void LookRight(float AxisValue);
	void Jump();
};
