#pragma once

#include "Data/Pawns/Creep.h" 
#include "Tool.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Turret.generated.h"

UCLASS()
class COLORDEFENSE_API ATurret : public ATool
{
    GENERATED_BODY()

public:
    ATurret();

protected:
    virtual void BeginPlay() override;
    virtual void LeftClick() override;
    virtual void RightClick() override;

public:
    virtual void Tick(float DeltaTime) override;

    // --- Stats ---
    UPROPERTY(EditAnywhere, Category = "Combat")
    float Range = 1000.f;

    // --- Components ---
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* TurretMesh; // Single mesh for the entire turret

private:
    void FindTarget();
    void RotateAndAttack(float DeltaTime);

    UPROPERTY()
    ACreep* CurrentTarget;
};