// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameEnums.h"
#include "SFX.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COLORDEFENSE_API USFX : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USFX();

	float ColorToPitch(M_CreepColor CreepColor);

	// 사운드
    UPROPERTY(EditAnywhere, Category = "Setting")
    TObjectPtr<class USoundBase> DestroySFX; 

	void PlayDestroySFX(M_CreepColor CreepColor, FVector ActorLocation);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:
    UFUNCTION()
    void OnOwnerDestroyed(AActor* DestroyedActor);
		
};
