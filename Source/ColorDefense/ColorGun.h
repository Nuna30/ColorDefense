// ColorGun.h
#pragma once

#include "CoreMinimal.h"
#include "Tool.h" // Inherit from Tool instead of SceneComponent
#include "Creep.h" 
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "ColorGun.generated.h"

UCLASS()
class COLORDEFENSE_API AColorGun : public ATool
{
    GENERATED_BODY()

public:
    AColorGun();
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime);
    virtual void LeftClick() override;

    UFUNCTION(BlueprintCallable, Category = "Action")
    void ChangeGunColor(EColor NewColor);

    // --- Chain Kill ---
    // Helper to check if a creep is already in the chain
    bool IsAlreadyConnected(ACreep* Creep);

    // Called when the mouse button is released
    virtual void LeftClickReleased(); 

    // Logic to add creeps to the chain during Tick
    void UpdateChain();

public:
    // --- Color Gun Specifics ---
    UPROPERTY(EditAnywhere, Category = "Setting")
    float MaxRange = 5000.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Setting")
    TObjectPtr<UNiagaraSystem> SwapVFX;

    // Since this is now an Actor, we use a SceneComponent as the root 
    // and a MeshComponent to show the gun model.
    UPROPERTY(VisibleAnywhere, Category = "Mesh")
    USceneComponent* DefaultRoot;

    UPROPERTY(VisibleAnywhere, Category = "Mesh")
    UStaticMeshComponent* GunMeshComponent;

    UPROPERTY(BlueprintReadOnly, Category = "Status")
    EColor CurrentColor = EColor::Red;

    // --- Chain kill ---
    UPROPERTY()
    TArray<ACreep*> ConnectedCreeps;

    // Flag to track the "Hold" state
    bool bIsConnecting = false;
};