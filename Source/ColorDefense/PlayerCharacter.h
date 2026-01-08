// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ColorGun.h" // ColorGun에게 캐릭터가 총을 쐈다는 신호를 전달하기 위해
#include "PlayerBlock.h"
#include "Camera/CameraComponent.h" // Adding the camera because the rest of the hierarchy is defined in C++.
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
    HoldingColorGun   UMETA(DisplayName = "Holding ColorGun"),
    HoldingBlock  UMETA(DisplayName = "Holding Block")
};

UCLASS()
class COLORDEFENSE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
public:
    // --- Components ---
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UCameraComponent* FPSCamera;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* InvisibleArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UChildActorComponent* ColorGunComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UChildActorComponent* PlayerBlockComponent;

    // --- Tool Logic ---
    UPROPERTY()
    ATool* CurrentTool; 

    // We store references to the actors managed by the components
    UPROPERTY()
    AColorGun* ColorGun; 

    UPROPERTY()
    APlayerBlock* PlayerBlock;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
    EPlayerState CurrentState;

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
	void Use();
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnShoot(); // 간단한 사격 애니메이션, 블루프린트에서 구현 됨 코드 없음

	// 컬러건 색깔 변경 바인딩
    void SetColorRed();
    void SetColorOrange();
    void SetColorYellow();
    void SetColorGreen();
    void SetColorBlue();
    void SetColorIndigo();
    void SetColorPurple();
    void RequestChangeColor(EColor NewColor);

    void SwitchToBlocks(); // Simplified toggle logic
    void SwitchTool();
};
