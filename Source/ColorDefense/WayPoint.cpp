// Fill out your copyright notice in the Description page of Project Settings.


#include "WayPoint.h"

// Sets default values
AWayPoint::AWayPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AWayPoint::BeginPlay()
{
	Super::BeginPlay();

	// 스태틱 메쉬 가시성 on / off
	UStaticMeshComponent* MeshComp = FindComponentByClass<UStaticMeshComponent>();
	if (VisibleInGame) MeshComp->SetVisibility(true);
	else MeshComp->SetVisibility(false);
	
}

// Called every frame
void AWayPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AWayPoint::GetLocation()
{
	return this->GetActorLocation();
}