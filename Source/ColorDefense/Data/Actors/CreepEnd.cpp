// Fill out your copyright notice in the Description page of Project Settings.

#include "CreepEnd.h"
#include "Data/Pawns/Creep.h" 
#include "GameStates/ColorDefenseGameState.h"
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
            // Destroy the collided creep.
            IncomingCreep->HandleDestruction();

            // Decrease the HP.
            if (AColorDefenseGameState* GS = GetWorld()->GetGameState<AColorDefenseGameState>()) GS->AddHP(-1);
        }
    }
}