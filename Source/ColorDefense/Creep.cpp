// Fill out your copyright notice in the Description page of Project Settings.


#include "Creep.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "WayPoint.h"

// Sets default values
ACreep::ACreep()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called every frame
void ACreep::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACreep::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Called when the game starts or when spawned
void ACreep::BeginPlay()
{
	Super::BeginPlay();

	// 위에서 BP_Creep 보기
	if (ViewInGame) 
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			PC->SetViewTarget(this);
		}	
	}
	
}

void ACreep::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

	// GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("AIController possesses BP_Creep"));
	// UE_LOG(LogTemp, Warning, TEXT("AIController possesses BP_Creep"));

	// AIController 가져오기
	AIController = Cast<AAIController>(GetController());

    if (AIController)
    {
		// 목적지에 도착했을 경우를 처리하는 OnMoveCompleted 콜백함수 연결
		AIController->ReceiveMoveCompleted.AddDynamic(this, &ACreep::OnMoveCompleted);

		// 출발
		if (DontMove == false)
		{
			FTimerHandle TimerHandle;
			GetWorldTimerManager().SetTimer(TimerHandle, this, &ACreep::MoveAlong, 0.1f, false);
		}
    }
	else // null일 수 있나...?
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("AIController is nullptr in ACreep::PossessedBy"));
		UE_LOG(LogTemp, Warning, TEXT("AIController is nullptr in ACreep::PossessedBy"));
	}
}


// Wapoint 차례대로 이동
void ACreep::MoveAlong()
{
	GetAllWaypoints();

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Waypoints.Num() = %d"), Waypoints.Num()));
	// UE_LOG(LogTemp, Warning, TEXT("Waypoints.Num() = %d"), Waypoints.Num());

    if (Waypoints.Num() > 0)
	{
		FVector StartLocation = Waypoints[0]->GetActorLocation();
		MoveTo(StartLocation.X, StartLocation.Y, StartLocation.Z);
	}
	else 
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("No Waypoint in Waypoints")));
		UE_LOG(LogTemp, Warning, TEXT("No Waypoint in Waypoints"));	
	}
}

// 모든 waypoint들 얻어서 Waypoints 배열에 저장
void ACreep::GetAllWaypoints()
{
	TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWayPoint::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		AWayPoint* Waypoint = Cast<AWayPoint>(Actor);
		if (Waypoint)
		{
			Waypoints.Add(Waypoint);
		}
		else 
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("No Waypoint")));
			UE_LOG(LogTemp, Warning, TEXT("No Waypoint"));
		}
	}
}

// 해당 좌표로 이동
void ACreep::MoveTo(float x, float y, float z)
{
	if (AIController)
	{
		FVector Destination = FVector(x, y, z);
		float AcceptanceRadius = VAcceptanceRadius;
		bool bStopOnOverlap = false;
		bool bUsePathfinding = true;
		bool bProjectDestinationToNavigation = false;
		bool bCanStrafe = false;
		TSubclassOf<UNavigationQueryFilter> FilterClass = nullptr;
		bool bAllowPartialPath = false;

		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("MoveToLocation...")));
		// UE_LOG(LogTemp, Warning, TEXT("MoveToLocation..."));

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

		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("MoveToLocation completed")));
		// UE_LOG(LogTemp, Warning, TEXT("MoveToLocation completed"));
	}
	else GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("No AIController")));

}

void ACreep::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("OnMoveCompleted...")));
	// UE_LOG(LogTemp, Warning, TEXT("OnMoveCompleted..."));

    CurrentWaypointIndex++;
    if (Waypoints.IsValidIndex(CurrentWaypointIndex))
    {
        FVector NextLocation = Waypoints[CurrentWaypointIndex]->GetActorLocation();
		// UE_LOG(LogTemp, Warning, TEXT("%s %f %f %f"), *GetActorLabel(), NextLocation.X, NextLocation.Y, NextLocation.Z);	
        MoveTo(NextLocation.X, NextLocation.Y, NextLocation.Z);
    }
	else GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("No Waypoints valid index")));

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("OnMoveCompleted completed")));
	// UE_LOG(LogTemp, Warning, TEXT("OnMoveCompleted completed"));
}