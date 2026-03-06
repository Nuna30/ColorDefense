// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Recoil.generated.h"


UCLASS()
class COLORDEFENSE_API URecoil : public UActorComponent
{
	GENERATED_BODY()
	URecoil();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;

public: // --- Properties --- //

	// --- Recoil --- //
	UPROPERTY(EditAnywhere, Category = "Settings")
	float VerticalRecoilPerShot = 1.2f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float HorizontalRecoilAmount = 0.65f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float RecoilRecoverySpeed = 7.5f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float MaxRecoilBuildup = 18.0f;

	UPROPERTY()
	float CurrentRecoilPitch = 0.0f;

	UPROPERTY()
	float CurrentRecoilYaw = 0.0f;

	UPROPERTY()
	ACharacter* OwnerCharacter = nullptr;

public: // --- Features --- //
	UFUNCTION()
	void ApplyRecoil();

	UFUNCTION()
	void ResetRecoil();
};
