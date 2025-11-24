// Fill out your copyright notice in the Description page of Project Settings.


#include "CreepGuide.h"
#include "Creep.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "CreepCheckPoint.h"

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
void UCreepGuide::GuideCreep(AAIController* P_AIController, bool DontMove) {
	UE_LOG(LogTemp, Error, TEXT("GuideCreep"));

	AIController = P_AIController;
    if (AIController)
    {
		// 목적지에 도착했을 경우를 처리하는 OnMoveCompleted 콜백함수 연결
		AIController->ReceiveMoveCompleted.AddDynamic(this, &UCreepGuide::OnMoveCompleted);

		// 출발
		if (DontMove == false)
		{
			FTimerHandle TimerHandle; // navmesh 계산 기다리기
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UCreepGuide::MoveAlong, 0.1f, false); // ActorComponent는 GetTimerManager를 못 씀
		}
    }
}

// 모든 CreepCheckPoint들 얻어서 CreepCheckPoints 배열에 저장
void UCreepGuide::GetAllCreepCheckPoints()
{
	TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACreepCheckPoint::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		ACreepCheckPoint* CreepCheckPoint = Cast<ACreepCheckPoint>(Actor);
		if (CreepCheckPoint) CreepCheckPoints.Add(CreepCheckPoint);
	}
}

// Wapoint 차례대로 이동
void UCreepGuide::MoveAlong()
{
	GetAllCreepCheckPoints();

    if (CreepCheckPoints.Num() > 0)
	{
		FVector StartLocation = CreepCheckPoints[0]->GetActorLocation();
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
    if (CreepCheckPoints.IsValidIndex(CurrentCreepCheckPointIndex))
    {
        FVector NextLocation = CreepCheckPoints[CurrentCreepCheckPointIndex]->GetActorLocation();
        MoveTo(NextLocation.X, NextLocation.Y, NextLocation.Z);
    }
}