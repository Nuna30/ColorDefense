// Tool.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tool.generated.h"

UCLASS()
class COLORDEFENSE_API ATool : public AActor
{
    GENERATED_BODY()

public:
    ATool(); // Added declaration

protected:
    virtual void BeginPlay() override; // Added declaration

public:
    virtual void Tick(float DeltaTime) override; // Added declaration

    virtual void Use() {}
    
    virtual void Equip() {SetActorHiddenInGame(false);}

    virtual void UnEquip() {SetActorHiddenInGame(true);}
};