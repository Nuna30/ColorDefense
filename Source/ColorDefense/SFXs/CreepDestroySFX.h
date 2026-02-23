// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Data/Pawns/Creep.h"  // 죽은 크립의 색깔과 위치를 알아내기 위해
#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Components/ActorComponent.h"
#include "CreepDestroySFX.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLORDEFENSE_API UCreepDestroySFX : public UActorComponent
{
	GENERATED_BODY()

public:
	// 사운드
    UPROPERTY(EditAnywhere, Category = "Setting")
    TObjectPtr<USoundBase> DestroySFX; 
	
public:	
	UCreepDestroySFX();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	float ColorToPitch(EColor CreepColor);	
	void PlayDestroySFX(EColor CreepColor, FVector ActorLocation);
	
    UFUNCTION()
    void OnOwnerDestroyed(AActor* DestroyedActor);
};
