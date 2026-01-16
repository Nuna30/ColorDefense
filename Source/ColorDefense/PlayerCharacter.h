// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ColorGun.h" // ColorGun에게 캐릭터가 총을 쐈다는 신호를 전달하기 위해
#include "PlayerBlock.h" // Using PlayerBlock
#include "Turret.h" // Using Turret
#include "Camera/CameraComponent.h" // Adding the camera because the rest of the hierarchy is defined in C++.
#include "InputActionValue.h" // Adding Enhance Input System.
#include "GameEnums.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class COLORDEFENSE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
protected:
    // --- Components ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCameraComponent* FPSCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* InvisibleArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UChildActorComponent* ColorGunComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UChildActorComponent* PlayerBlockComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UChildActorComponent* TurretComponent;

protected:
    /** MappingContext for player input */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputMappingContext* DefaultMappingContext;

    /** Action for changing colors */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* ChangeColorAction;

    /** Action for switching tools */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* SwitchToolAction;

    // The single function that will handle all colors via a value
    void HandleChangeColor(const FInputActionValue& Value);

    // The single function that will handle all Tools via a value
    void HandleSwitchTool(const FInputActionValue& Value);
    
protected:
    // --- Tool Logic ---
    UPROPERTY()
    TArray<ATool*> Tools;

    UPROPERTY()
    ATool* CurrentTool; 

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
    EPlayerState CurrentState;

    // We store references to the actors managed by the components
    UPROPERTY()
    AColorGun* ColorGun; 

    UPROPERTY()
    APlayerBlock* PlayerBlock;

    UPROPERTY()
    ATurret* Turret;

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// 키 바인딩 wasd 마우스
	void MoveForward(float AxisValue);
	void LookUp(float AxisValue);
	void MoveRight(float AxisValue);
	void LookRight(float AxisValue);
	void Jump();
	
	UFUNCTION(BlueprintCallable)
	void HandleLeftClick();
    
    UFUNCTION(BlueprintCallable)
	void HandleRightClick();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnShoot(); // 간단한 사격 애니메이션, 블루프린트에서 구현 됨 코드 없음
};
