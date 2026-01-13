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

    // --- Polymorphic Override ---
    // This is called when PlayerCharacter calls CurrentTool->LeftClick()
    virtual void LeftClick() override;

    UFUNCTION(BlueprintCallable, Category = "Action")
    void ChangeGunColor(EColor NewColor);

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
};