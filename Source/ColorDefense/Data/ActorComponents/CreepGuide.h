#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CreepGuide.generated.h"


UCLASS()
class COLORDEFENSE_API UCreepGuide : public UActorComponent
{
	GENERATED_BODY()
	UCreepGuide();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public: // --- Property --- //
	// ! EditAnywhere properties only appear in the Details panel of the class where they are actually declared.
	// ! So these variables doesn't appear in the BP_Creep.
	// ! These variables are initialized in the Creep source code.
	float MoveSpeed;
	float AcceptanceRadius; // (cm)

public: // --- Creep Check Point --- //
	// CheckPointIndex last passed by the creep
	int CurrentCreepCheckPointIndex = 0;

	// Needed to get the Creep CheckPoints.
	int32 RailNumber;

	// CreepCheckPointGenerator has CreepCheckPoints.
	class UCreepCheckPointGenerator* CreepCheckPointGenerator;

public: // --- Movement --- //
	FVector CurrentTargetLocation;
	bool bIsMovingToCheckPoint = false;
	
	void GuideCreep(bool DontMove, int32 InRailNumber);
	void UpdateNextCheckPoint(float x, float y, float z);
	void TickMoveCreep(float DeltaTime);
    void OnCheckPointReached();
};
