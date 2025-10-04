// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Turret.generated.h"

UCLASS()
class COLORDEFENSE_API ATurret : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// collider를 캡슐로
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	class UCapsuleComponent* CapsuleComp;

	// 터렛 메쉬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	UStaticMeshComponent* TurretMesh;

	// 빔 나가는 곳
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Components")
	USceneComponent* TurretLaser;

	// 터렛 범위 지정
	UPROPERTY(EditAnywhere, Category = "Setting")
	float TurretRange = 600;

	// 크립들을 모아두는 풀
	class UCreepPoolSubsystem* CreepPool;

private:
	// 터렛이 TargetLocation을 바라보게 만들기
	void RotateTurret(FVector TargetLocation);
	// 터렛이 범위 내에 Creep이 있는지 확인 후 RotateTurret 수행
	void DetectCreep();
};