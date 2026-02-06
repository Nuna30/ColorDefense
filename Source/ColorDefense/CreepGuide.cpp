// Fill out your copyright notice in the Description page of Project Settings.


#include "CreepGuide.h"


// Sets default values for this component's properties
UCreepGuide::UCreepGuide()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UCreepGuide::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UCreepGuide::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// I will guide you.
void UCreepGuide::GuideCreep(AAIController* InAIController, bool DontMove, int32 InRailNumber) {
	this->RailNumber = InRailNumber;
	this->AIController = InAIController;

    if (AIController)
    {
		// 목적지에 도착했을 경우를 처리하는 OnMoveCompleted 콜백함수 연결
		AIController->ReceiveMoveCompleted.AddDynamic(this, &UCreepGuide::OnMoveCompleted);

		// Get CreepCheckPointGenerator
		UCreepCheckPointGeneratorManager* CreepCheckPointGeneratorManager = GetWorld()->GetGameInstance()->GetSubsystem<UCreepCheckPointGeneratorManager>();
		TArray<UCreepCheckPointGenerator*> CreepCheckPointGenerators = CreepCheckPointGeneratorManager->CreepCheckPointGenerators;
		this->CreepCheckPointGenerator = CreepCheckPointGenerators[this->RailNumber];

		// 출발
		if (DontMove == false)
		{
			FTimerHandle TimerHandle; // navmesh 계산 기다리기
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UCreepGuide::MoveAlong, 0.1f, false);
		}
    }
}

// Waypoint 차례대로 이동
void UCreepGuide::MoveAlong()
{
    if (this->CreepCheckPointGenerator->CreepCheckPointLocations.Num() > 0)
	{
		FVector StartLocation = this->CreepCheckPointGenerator->CreepCheckPointLocations[0];
		MoveTo(StartLocation.X, StartLocation.Y, StartLocation.Z);
	}
}

// 해당 좌표로 이동
void UCreepGuide::MoveTo(float x, float y, float z)
{
	if (AIController)
	{
		FVector Destination = FVector(x, y, z);
		float AcceptanceRadius = 1;
		bool bStopOnOverlap = false;
		bool bUsePathfinding = true;
		bool bProjectDestinationToNavigation = false;
		bool bCanStrafe = false;
		TSubclassOf<UNavigationQueryFilter> FilterClass = nullptr;
		bool bAllowPartialPath = false;

		EPathFollowingRequestResult::Type Result = AIController->MoveToLocation(
			Destination,
			AcceptanceRadius,
			bStopOnOverlap,
			bUsePathfinding,
			bProjectDestinationToNavigation,
			bCanStrafe,
			FilterClass,
			bAllowPartialPath
		);
	}
}

void UCreepGuide::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
    CurrentCreepCheckPointIndex++;
    if (this->CreepCheckPointGenerator->CreepCheckPointLocations.IsValidIndex(CurrentCreepCheckPointIndex))
    {
        FVector NextLocation = this->CreepCheckPointGenerator->CreepCheckPointLocations[CurrentCreepCheckPointIndex];
        MoveTo(NextLocation.X, NextLocation.Y, NextLocation.Z);
    }
}