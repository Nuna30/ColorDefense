// Fill out your copyright notice in the Description page of Project Settings.


#include "CreepCheckPoint.h"

// Sets default values
ACreepCheckPoint::ACreepCheckPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACreepCheckPoint::BeginPlay()
{
	Super::BeginPlay();

	// 스태틱 메쉬 가시성 on / off
	UStaticMeshComponent* MeshComp = FindComponentByClass<UStaticMeshComponent>();
	if (VisibleInGame) MeshComp->SetVisibility(true);
	else MeshComp->SetVisibility(false);
	
}

// Called every frame
void ACreepCheckPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ACreepCheckPoint::GetLocation()
{
	return this->GetActorLocation();
}