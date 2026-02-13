// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CreepEnd.generated.h"

UCLASS()
class COLORDEFENSE_API ACreepEnd : public AActor
{
	GENERATED_BODY()
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	ACreepEnd();

public:	
	UPROPERTY(EditAnywhere, Category = "Components")
    UStaticMeshComponent* MeshComponent;

public:
	UFUNCTION()
    void OnOverlapBegin
	(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, 
        bool bFromSweep,
		const FHitResult& SweepResult
	);
};
