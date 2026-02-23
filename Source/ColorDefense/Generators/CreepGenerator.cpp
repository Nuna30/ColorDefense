// Fill out your copyright notice in the Description page of Project Settings.

#include "Generators/CreepGenerator.h"
#include "GeneratorManagers/CreepPatternGeneratorManager.h"
#include "CreepPatternGenerator.h"

ACreepGenerator::ACreepGenerator()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACreepGenerator::BeginPlay()
{
	Super::BeginPlay();

    this->CreepPatternGenerator = GetWorld()->GetSubsystem<UCreepPatternGeneratorManager>()->CreepPatternGenerator;
}

void ACreepGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACreepGenerator::GenerateCreeps()
{
    // Generate Creep Pattern if it is empty.
    if (this->CreepPatternGenerator->IsEmpty(this->RailNumber)) this->CreepPatternGenerator->GeneratePattern();

    // Get spawn info and spawn a creep referring to spawn info.
    FSpawnInfo SpawnInfo = this->CreepPatternGenerator->PopSpawnInfo(this->RailNumber);
    SpawnCreep(SpawnInfo.CreepColorNumber);

    // Call this function again after SpawnInfo.SpawnDelay.
    GetWorld()->GetTimerManager().SetTimer(
		this->CreepGenerationTimer,        
		this,              
		&ACreepGenerator::GenerateCreeps, 
		SpawnInfo.SpawnDelay,               
		false                
	);
}

void ACreepGenerator::SpawnCreep(int32 ColorNumber)
{
	// Set up the spawn info.
	FVector SpawnLocation = GetActorLocation();
	FRotator SpawnRotation = FRotator::ZeroRotator;
	FTransform SpawnTransform(SpawnRotation, SpawnLocation);

	// Use deferred spawn.
    ACreep* Creep = GetWorld()->SpawnActorDeferred<ACreep>(
        CreepClass, 
        SpawnTransform, 
        this,
        nullptr, 
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn
    );

    // Sync the Creep's rail number.
    Creep->RailNumber = this->RailNumber;

    // Sync the Creep's color
    switch (ColorNumber) {
        case 1 : Creep->ChangeColor(EColor::Red); break;
        case 2 : Creep->ChangeColor(EColor::Orange); break;
        case 3 : Creep->ChangeColor(EColor::Yellow); break;
        case 4 : Creep->ChangeColor(EColor::Green); break;
        case 5 : Creep->ChangeColor(EColor::Blue); break;
        case 6 : Creep->ChangeColor(EColor::Purple); break;
        case 7 : Creep->ChangeColor(EColor::Indigo); break;
    }

    // Finish the deferred spawn.
    UGameplayStatics::FinishSpawningActor(Creep, SpawnTransform);
}

void ACreepGenerator::SetRailNumber(int32 InRailNumber)
{
	this->RailNumber = InRailNumber;
}