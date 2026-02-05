// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CreepCheckPointGeneratorManager.h" // CreepCheckPointGeneratorManager한테서 CreepCheckPoint들의 좌표를 얻기 위해
#include "AIController.h" // 빙의한 Creep의 AIController로 Creep을 주어진 좌표로 이동시키기 위해
#include "Navigation/PathFollowingComponent.h" // Creep이 주어진 좌표에 도착했다는 결과를 사용하기 위해
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CreepGuide.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLORDEFENSE_API UCreepGuide : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:		
	// 현재 진행 중인 웨이포인트의 인덱스
	int CurrentCreepCheckPointIndex = 0;

	// 빙의한 Creep의 AIController로 Creep 조종
	AAIController* AIController;

	// 빙의한 Creep의 Rail을 얻어서 해당 Rail의 CheckPoint만 얻어야 함
	int32 RailNumber;

	class UCreepCheckPointGenerator* CreepCheckPointGenerator;
	
public:
	UCreepGuide();

	// Creep guide 하기
	void GuideCreep(AAIController* InAIController, bool DontMove, int32 InRailNumber);

	// 해당 좌표로 BP_Creep 이동
	void MoveTo(float x, float y, float z);
	
	// CreepCheckPoint들을 따라 이동
	void MoveAlong();

	// Wapoint 도착 시 실행될 콜백함수
    UFUNCTION()
    void OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
