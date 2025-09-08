// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WayPoint.generated.h"

UCLASS()
class COLORDEFENSE_API AWayPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWayPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Option")
	bool VisibleInGame = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Waypoiny의 좌표 반환
	FVector GetLocation();

};
