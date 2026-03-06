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

USTRUCT()
struct FHoldInfo
{
    GENERATED_BODY()

    UPROPERTY()
    ATool* Tool;

    UPROPERTY()
    EPlayerState PlayerState;
};


UCLASS()
class COLORDEFENSE_API AToolBox : public AActor
{
	GENERATED_BODY()

public: // --- Base functions --- //
	AToolBox();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public: // --- Properties --- //
    UPROPERTY()
    TArray<FHoldInfo> HoldInfoArray;

    UPROPERTY()
    FHoldInfo CurrentHoldInfo;

public: // --- Delegate --- //
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnToolChanged, int32, NewIndex); // Primarily used in PlayerCharacter.
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnToolHoldInfoUpdated);

	UPROPERTY()
	FOnToolChanged OnToolChanged;
	UPROPERTY()
	FOnToolHoldInfoUpdated OnToolHoldInfoUpdated;

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

public: // --- Bind Mapping --- //

	// --- Input Actions --- //
	UPROPERTY(EditAnywhere, Category = "Setting")
	UInputAction* ChangeColorAction;

	UPROPERTY(EditAnywhere, Category = "Setting")
	UInputAction* SwitchToolAction;

	// --- Input Binding --- //
	void BindActions(UInputComponent* PlayerInputComponent, class APlayerCharacter* OwnerCharacter);
	void BindToolActionsEnhanced(class APlayerCharacter* OwnerCharacter);
	    
	// --- Handlers --- //
	void HandleLeftClick();
	void HandleLeftClickReleased();
	void HandleRightClick();
    void HandleChangeColor(const FInputActionValue& Value);
    void HandleSwitchTool(const FInputActionValue& Value);

public: // --- Utils --- //
	void SwapTools(int32 IndexA, int32 IndexB);
	void SwitchTools(int32 NewToolIndex);
};
