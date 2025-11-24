// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Pawn.h"
#include "Navigation/PathFollowingComponent.h"
#include "CreepGuide.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLORDEFENSE_API UCreepGuide : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCreepGuide();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:		
	// 웨이포인트 저장
	TArray<class ACreepCheckPoint*> CreepCheckPoints;

	// 현재 진행 중인 웨이포인트의 인덱스
	int CurrentCreepCheckPointIndex = 0;

	// Creep guide 하기
	void GuideCreep(class AAIController* AIController, bool DontMove);

	// 모든 CreepCheckPoint들 얻어서 CreepCheckPoints 배열에 저장
	void GetAllCreepCheckPoints();

	// 해당 좌표로 BP_Creep 이동
	void MoveTo(float x, float y, float z);
	
	// CreepCheckPoint들을 따라 이동
	void MoveAlong();

	// Wapoint 도착 시 실행될 콜백함수
    UFUNCTION()
    void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	class AAIController* AIController;
};
