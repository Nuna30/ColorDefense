#include "Creep.h"
#include "Data/Actors/CreepCore.h"
#include "Data/Actors/CreepShield.h"
#include "Utils/Utils.h"

ACreep::ACreep()
{
	PrimaryActorTick.bCanEverTick = false;

    // IMPORTANT: Using an explicit USceneComponent as the root is crucial for proper attachment
    // and movement of child components (Core and Shield). See dev log from Feb 26, 2026, for details
    // on the bug fix and reasoning.
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    // Creep Core
    CreepCoreComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("CreepCore"));
    CreepCoreComponent->SetupAttachment(RootComponent);

    // Creep Shield
    CreepShieldComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("CreepShield"));
    CreepShieldComponent->SetupAttachment(RootComponent);

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

    // Change the CreepCore's color.
    if (ACreepCore* CreepCore = Cast<ACreepCore>(CreepCoreComponent->GetChildActor()))
    {
        CreepCore->ChangeColor(CreepColor);
    }

    // Change the CreepShield's color.
    if (ACreepShield* CreepShield = Cast<ACreepShield>(CreepShieldComponent->GetChildActor()))
    {
        CreepShield->ChangeColor(CreepColor);
    }
}