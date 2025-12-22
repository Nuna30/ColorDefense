// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Creep.h" // 크립을 터트리기 위해
#include "CoreMinimal.h"
#include "ColorGun.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLORDEFENSE_API UColorGun : public USceneComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// ColorGun 최대 사거리
	UPROPERTY(EditAnywhere, Category = "Setting")
	float MaxRange = FLT_MAX;
	
public:	
	UColorGun();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// 색깔 총 발사
	void Shoot();
};
