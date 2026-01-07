#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Creep.h"
#include "Turret.generated.h"

UCLASS()
class COLORDEFENSE_API ATurret : public AActor
{
    GENERATED_BODY()

public:
    ATurret();

protected:
    virtual void BeginPlay() override;

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