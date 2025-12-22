// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CreepGuide.h" // 크립을 레일을 따라 이동시키기 위해
#include "AIController.h" // 크립의 제어권을 AI에게 넘기기 위해
#include "NiagaraFunctionLibrary.h" // VFX를 크립이 죽을 때 스폰시키기 위해
#include "NiagaraComponent.h" // VFX 색깔 바꾸기 위해
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Creep.generated.h"

UCLASS()
class COLORDEFENSE_API ACreep : public APawn
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	// vfx
    UPROPERTY(EditDefaultsOnly, Category = "Setting")
    TObjectPtr<UNiagaraSystem> VFX;

	// Mesh 컴포넌트
    UPROPERTY(EditAnywhere, Category = "Setting")
    UStaticMeshComponent* CreepMesh;

	// vfx, sfx에서 쓰이는 색깔
	EColor CreepColor = EColor::Red;

	// 어느 레일에 속하는지
	int32 RailNumber;

	// AI Controller
	AAIController* AIController;

	// Creep을 레일을 따라 이동시키게 만들어주는 크립가이드
	UPROPERTY()
	UCreepGuide* CreepGuide;

	// 테스트용, 크립 에셋을 레벨에 설치해서 다룰 때 움직이지 않도록 설정할 수 있게 함
	UPROPERTY(EditAnywhere, Category = "Setting")
	bool DontMove = false;

public:
	ACreep();

	// AI Controller가 안전하게 할당되고 크립을 이동시키는 함수
	virtual void PossessedBy(AController* NewController) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 크립 파괴 시
	void HandleDestruction();

	// 크립 색상 동적 변경
	void ChangeColor(EColor Color);
};
