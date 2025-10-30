// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Navigation/PathFollowingComponent.h"
#include "GameEnums.h"
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

	// vfx
    UPROPERTY(EditDefaultsOnly, Category = "Setting")
    TObjectPtr<class UNiagaraSystem> VFX;

	// // 사운드
    // UPROPERTY(EditDefaultsOnly, Category = "Setting")
    // TObjectPtr<class USoundBase> SFX; 

	// Mesh 컴포넌트
    UPROPERTY(EditAnywhere, Category = "Setting")
    UStaticMeshComponent* CreepMesh;

private:
	// AI Controller
	class AAIController* AIController;

	// AI Controller가 안전하게 할당되고 크립을 이동시키는 함수
	virtual void PossessedBy(AController* NewController) override;

	// Creep을 이동시키는 로직
	UPROPERTY()
	class UCreepGuide* CreepGuide;

	UPROPERTY(EditAnywhere, Category = "Setting")
	bool DontMove = false;

	// 크립들을 모아두는 풀
	class UCreepPoolSubsystem* CreepPool;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ChangeColor(M_CreepColor Color);

	void HandleDestruction();

	// 크립 색깔
	M_CreepColor CreepColor = M_CreepColor::Red;
};
