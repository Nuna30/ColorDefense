// Tool.h
#pragma once

#include "Utils/GameEnums.h"
#include "Utils/Utils.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// --- Base Functions --- //
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tool.generated.h"

UENUM()
enum class ETools : uint8
{
    CoreRemover,
    PlayerBlock,
    Turret
};

UCLASS()
class COLORDEFENSE_API ATool : public AActor
{
    GENERATED_BODY()

public: // --- Mesh --- //
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting") // BlueprintReadWrite to implement OnShoot animation on blueprint.
    UStaticMeshComponent* ToolMeshComponent;

public: // --- Properties --- //
    EColor CurrentColor;

public: // --- Base functions --- //
    ATool(); 
    virtual void BeginPlay() override; 
    virtual void Tick(float DeltaTime) override;

public: // --- Handle Mouse Input --- //
    virtual void LeftClick();
    virtual void LeftClickReleased();
    virtual void RightClick();

public: // --- Utils --- //
    virtual void SwitchToolFrom(ATool*& CurrentTool);
    virtual void UnEquip();

public: // --- Features --- //

    // --- Swap Color --- //
    UPROPERTY(EditDefaultsOnly, Category = "Setting")
    TObjectPtr<UNiagaraSystem> SwapVFX;

    virtual void ChangeColor(EColor NewColor);

    // --- ... --- //
};