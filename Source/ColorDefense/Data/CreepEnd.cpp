// Fill out your copyright notice in the Description page of Project Settings.

#include "CreepEnd.h"
#include "Creep.h"
#include "Components/StaticMeshComponent.h"

ACreepEnd::ACreepEnd()
{
    PrimaryActorTick.bCanEverTick = false;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // Enable overlap events for this component
    MeshComponent->SetGenerateOverlapEvents(true);
}

void ACreepEnd::BeginPlay()
{
	Super::BeginPlay();

	if (MeshComponent)
    {
        MeshComponent->OnComponentBeginOverlap.AddDynamic(this, &ACreepEnd::OnOverlapBegin);
    }
}

void ACreepEnd::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACreepEnd::OnOverlapBegin
(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult& SweepResult
)
{
    if (OtherActor && (OtherActor != this))
    {
        ACreep* IncomingCreep = Cast<ACreep>(OtherActor);
        if (IncomingCreep)
        {
            // The creep's mesh has hit your CreepEnd's mesh
            IncomingCreep->HandleDestruction();
            UE_LOG(LogTemp, Log, TEXT("Creep reached the exact mesh boundary!"));
        }
    }
}