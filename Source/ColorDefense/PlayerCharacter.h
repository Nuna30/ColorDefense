// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ColorGun.h" // ColorGun에게 캐릭터가 총을 쐈다는 신호를 전달하기 위해
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class COLORDEFENSE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	// 캐릭터가 쏘는 총
	UColorGun* ColorGun;

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
	void Shoot();
	
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

};
