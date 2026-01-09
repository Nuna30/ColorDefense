// Tool.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tool.generated.h"

UENUM(BlueprintType)
enum class ETools : uint8
{
    ColorGun   UMETA(DisplayName = "Color Gun"),
    PlayerBlock  UMETA(DisplayName = "Player Block"),
    Turret  UMETA(DisplayName = "Turret")
};

UCLASS()
class COLORDEFENSE_API ATool : public AActor
{
    GENERATED_BODY()

public:
    ATool(); 



protected:
    virtual void BeginPlay() override; 

public:
    virtual void Tick(float DeltaTime) override;

    virtual void Use() {}
    
    virtual void SwitchToolFrom(ATool*& CurrentTool);

    virtual void UnEquip() {SetActorHiddenInGame(true);}
};