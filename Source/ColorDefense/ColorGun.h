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

	// 총의 메쉬 컴포넌트
	UStaticMeshComponent* GunMeshComponent;

	// 현재 선택된 총의 색상
	// 이 색상을 보고 크립을 터트릴지말지 결정됨
	UPROPERTY(BlueprintReadOnly, Category = "Status")
	EColor CurrentColor = EColor::Red;
	
public:	
	UColorGun();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// 색깔 총 발사
	void Shoot();

	// 총의 색상을 변경하는 함수 (키보드 입력 1~7로 호출)
	UFUNCTION(BlueprintCallable, Category = "Action")
	void ChangeGunColor(EColor NewColor);
};
