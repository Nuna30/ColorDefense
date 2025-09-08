// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Navigation/PathFollowingComponent.h"
#include "Creep.generated.h"

UCLASS()
class COLORDEFENSE_API ACreep : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACreep();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// AI Controller
	class AAIController* AIController;

	// AI Controller가 안전하게 할당되고 크립을 이동시키는 함수
	virtual void PossessedBy(AController* NewController) override;

	// 웨이포인트 저장
	TArray<class AWayPoint*> Waypoints;
	
	// 현재 진행 중인 웨이포인트의 인덱스
	int CurrentWaypointIndex = 0;

	// 모든 waypoint들 얻어서 Waypoints 배열에 저장
	void GetAllWaypoints();

	// 해당 좌표로 BP_Creep 이동
	void MoveTo(float x, float y, float z);
	
	// WayPoint들을 따라 이동
	void MoveAlong();

	// Wapoint 도착 시 실행될 콜백함수
    UFUNCTION()
    void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	// AcceptanceRadius 조절 변수
	UPROPERTY(EditAnywhere, Category = "Setting")
	float VAcceptanceRadius = 1;

	// 인게임에서 BP_Creep을 위에선 내려다볼 수 있게 만든 변수
	UPROPERTY(EditAnywhere, Category = "Setting")
	bool ViewInGame = false;

	UPROPERTY(EditAnywhere, Category = "Setting")
	bool DontMove = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
