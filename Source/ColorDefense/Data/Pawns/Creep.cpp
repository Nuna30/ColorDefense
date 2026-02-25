#include "Creep.h"
#include "Data/Actors/CreepCore.h"
#include "Utils/Utils.h"

ACreep::ACreep()
{
	PrimaryActorTick.bCanEverTick = false;

    // Creep Core Component
    CreepCoreComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("CreepCore"));
    CreepCoreComponent->SetupAttachment(RootComponent);

    // Creep Guide
	CreepGuide = CreateDefaultSubobject<UCreepGuide>(TEXT("CreepGuide"));
}

void ACreep::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACreep::BeginPlay()
{
	Super::BeginPlay();

    CreepGuide->GuideCreep(DontMove, RailNumber);

    CreepGuide->MoveSpeed         = CreepMoveSpeed;
    CreepGuide->AcceptanceRadius  = CreepAcceptanceRadius;
}

void ACreep::HandleDestruction()
{
    if (ACreepCore* CreepCore = Cast<ACreepCore>(CreepCoreComponent->GetChildActor()))
    {
        CreepCore->HandleDestruction();
    }

	Destroy();
}

void ACreep::ChangeColor(EColor Color)
{
    CreepColor = Color;

    // Change the CreepColor's color.
    if (ACreepCore* CreepCore = Cast<ACreepCore>(CreepCoreComponent->GetChildActor()))
    {
        CreepCore->ChangeColor(CreepColor);
    }
}