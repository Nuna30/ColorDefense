// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"
#include "Creep.h"
#include "CreepPoolSubsystem.h"

// Sets default values
ATurret::ATurret()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Collider"));
	RootComponent = CapsuleComp;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret Mesh"));
	TurretMesh->SetupAttachment(CapsuleComp);

	TurretLaser = CreateDefaultSubobject<USceneComponent>(TEXT("Turret Laser"));
	TurretLaser->SetupAttachment(TurretMesh);
}

// Called when the game starts or when spawned
void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
	// 크립 객체 포인터들을 모아두는 풀 생성
	CreepPool = GetGameInstance()->GetSubsystem<UCreepPoolSubsystem>();
}

// Called every frame
void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 터렛이 범위 내에 Creep이 있는지 확인 후 RotateTurret 수행
	DetectCreep();
}

void ATurret::RotateTurret(FVector TargetLocation)
{
	FVector ToTarget = TargetLocation - TurretLaser->GetComponentLocation();
	FRotator ToTargetRotation = ToTarget.Rotation();
	TurretLaser->SetWorldRotation(ToTargetRotation);
}

// 터렛이 범위 내에 Creep이 있는지 확인 후 RotateTurret 수행
void ATurret::DetectCreep()
{
	const TArray<ACreep*>& Creeps = CreepPool->GetCreeps();
	float MinDist = BIG_NUMBER;
	ACreep* ClosestCreep = nullptr;

	for (ACreep* Creep : Creeps)
	{
		float Dist = (Creep->GetActorLocation() - TurretLaser->GetComponentLocation()).Size();
		if (Dist < MinDist)
		{
			MinDist = Dist;
			ClosestCreep = Creep;
		}
	}

	if (ClosestCreep) RotateTurret(ClosestCreep->GetActorLocation());
}


