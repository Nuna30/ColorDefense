#include "CreepGuide.h"
#include "GeneratorManagers/CreepCheckPointGeneratorManager.h"

UCreepGuide::UCreepGuide()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCreepGuide::BeginPlay()
{
	Super::BeginPlay();
}

void UCreepGuide::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsMovingToCheckPoint) return;
	if (!CreepCheckPointGenerator) return;

	TickMoveCreep(DeltaTime);
}

void UCreepGuide::GuideCreep(bool DontMove, int32 InRailNumber) {
	// For Debugging
	if (DontMove) return;

	// Set the Creep's rail number.
	this->RailNumber = InRailNumber;

	// Get CreepCheckPointGenerator.
	UCreepCheckPointGeneratorManager* CreepCheckPointGeneratorManager = GetWorld()->GetSubsystem<UCreepCheckPointGeneratorManager>();
	TArray<UCreepCheckPointGenerator*> CreepCheckPointGenerators = CreepCheckPointGeneratorManager->CreepCheckPointGenerators;
	this->CreepCheckPointGenerator = CreepCheckPointGenerators[this->RailNumber];

	// Initialize the properties.
	CurrentCreepCheckPointIndex = 0;
	bIsMovingToCheckPoint = false;

	// Start guiding the creep.
	FVector StartLocation = this->CreepCheckPointGenerator->CreepCheckPointLocations[0];
	UpdateNextCheckPoint(StartLocation.X, StartLocation.Y, StartLocation.Z);
}

void UCreepGuide::UpdateNextCheckPoint(float x, float y, float z)
{
	// Change the target location.
	CurrentTargetLocation = FVector(x, y, z);
	bIsMovingToCheckPoint = true;
}

void UCreepGuide::TickMoveCreep(float DeltaTime)
{
	// Get Pawn.
	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	// Calculate remaining distance.
	FVector CurrentLocation = OwnerPawn->GetActorLocation();
	float DistanceToTarget = FVector::Dist(CurrentLocation, CurrentTargetLocation);

	// Handle the case that Creep arrives at next checkpoint.
	if (DistanceToTarget <= AcceptanceRadius)
	{
		bIsMovingToCheckPoint = false;
		OnCheckPointReached();
	}
	// Handle the case that Creep is moving to the next checkpoint.
	else
	{
		// Calculate travel distance.
		float MoveDistance = MoveSpeed * DeltaTime;
		
		// Prevent overshooting.
		if (MoveDistance > DistanceToTarget) MoveDistance = DistanceToTarget;
		
		// Calculate Creep's new location.
		FVector Direction = (CurrentTargetLocation - CurrentLocation).GetSafeNormal();
		FVector NewLocation = CurrentLocation + Direction * MoveDistance;

		// Apply the new location.
		FHitResult HitResult;
		OwnerPawn->SetActorLocation(NewLocation, true, &HitResult);
	}
}

void UCreepGuide::OnCheckPointReached()
{
	// If the creep reached the current check point, move to the next check point.
    CurrentCreepCheckPointIndex++;
    if (this->CreepCheckPointGenerator->CreepCheckPointLocations.IsValidIndex(CurrentCreepCheckPointIndex))
    {
        FVector NextLocation = this->CreepCheckPointGenerator->CreepCheckPointLocations[CurrentCreepCheckPointIndex];
        UpdateNextCheckPoint(NextLocation.X, NextLocation.Y, NextLocation.Z);
    }
	// If the creep has reached the end,
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Creep reached the end of the path on rail %d"), RailNumber);
		//I can add some final arrival logics here.
    }
}