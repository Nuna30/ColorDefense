// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CreepPoolSubsystem.generated.h"

class ACreep;

UCLASS()
class COLORDEFENSE_API UCreepPoolSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	

public:
	void AddCreep(ACreep* Creep);
	void RemoveCreep(ACreep* Creep);
	const TArray<ACreep*>& GetCreeps() const {return Creeps;}
	void printCreeps();

private:
	UPROPERTY()
	TArray<ACreep*> Creeps;
};
