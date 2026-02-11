// ColorGun.h
#pragma once

#include "Tool.h" 
#include "Creep.h" 
#include "ColorDefenseGameState.h"
#include "Widgets/ComboWidget.h"
#include "Widgets/MoneyWidget.h"
#include "Components/AudioComponent.h"
#include "CoreMinimal.h"
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

    // --- Combo ---
    void PlayComboSound();

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

    // --- Combo ---
    int32 TotalCombo = 0;
    int32 ComboCount = 0;

    UPROPERTY(EditAnywhere, Category = "Setting")
    USoundBase* ComboSound;

    // The component that manages the active sound
    UPROPERTY(EditAnywhere, Category = "Setting")
    UAudioComponent* ComboAudioComponent;

    // Control how much the pitch increases per combo
    UPROPERTY(EditAnywhere, Category = "Setting")
    float PitchMultiplier = 0.1f;

    // --- Combo UI ---
    UPROPERTY(EditAnywhere, Category = "Setting")
    TSubclassOf<UComboWidget> ComboWidgetClass;

    UPROPERTY()
    UComboWidget* ComboWidgetInstance;

        // --- Money UI ---
    UPROPERTY(EditAnywhere, Category = "Setting")
    TSubclassOf<UMoneyWidget> MoneyWidgetClass;

    UPROPERTY()
    UMoneyWidget* MoneyWidgetInstance;
};