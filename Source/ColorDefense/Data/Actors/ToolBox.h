#pragma once

// --- Tools --- //
#include "Tools/CoreRemover.h"
#include "Tools/Rifle.h"
#include "Tools/PlayerBlock.h"
#include "Tools/Turret.h"

// --- Input System --- //
#include "InputAction.h"               // for UInputAction
#include "InputActionValue.h"          // for FInputActionValue
#include "Components/InputComponent.h" // for UInputComponent
#include "EnhancedInputComponent.h"	   // for UEnhancedInputComponent

// --- ETC --- //
#include "Utils/GameEnums.h"

// --- Default --- //
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ToolBox.generated.h"


UCLASS()
class COLORDEFENSE_API AToolBox : public AActor
{
	GENERATED_BODY()

public: // --- Base functions --- //
	AToolBox();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:  // --- Tools --- //
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
    UChildActorComponent* CoreRemoverComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
    UChildActorComponent* RifleComponent;

    // UPROPERTY(EditAnywhere, Category = "Setting")
    // UChildActorComponent* PlayerBlockComponent;

    // UPROPERTY(EditAnywhere, Category = "Setting")
    // UChildActorComponent* TurretComponent;

	UPROPERTY()
	ACoreRemover* CoreRemover;

	UPROPERTY()
	ARifle* Rifle;

	// UPROPERTY()
	// APlayerBlock* PlayerBlock;

	// UPROPERTY()
	// ATurret* Turret;

public: // --- Tool Box --- //
    UPROPERTY()
    TArray<ATool*> ToolBox;

public: // --- Properties --- //
    UPROPERTY()
    ATool* CurrentTool; 

    UPROPERTY()
    EPlayerState CurrentState;

public: // --- Bind Mapping --- //

	// --- Input Actions --- //
	UPROPERTY(EditAnywhere, Category = "Setting")
	UInputAction* ChangeColorAction;

	UPROPERTY(EditAnywhere, Category = "Setting")
	UInputAction* SwitchToolAction;

	// --- Input Binding --- //
	void BindActions(UInputComponent* PlayerInputComponent, class APlayerCharacter* OwnerCharacter);
	void BindCoreRemoverActionsEnhanced(class APlayerCharacter* OwnerCharacter);
	    
	// --- Handlers --- //
	void HandleLeftClick();
	void HandleLeftClickReleased();
	void HandleRightClick();
    void HandleChangeColor(const FInputActionValue& Value);
    void HandleSwitchTool(const FInputActionValue& Value);
};
