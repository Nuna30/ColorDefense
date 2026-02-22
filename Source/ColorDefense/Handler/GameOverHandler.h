#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameOverHandler.generated.h"

// Forward declaration to avoid circular dependencies
class UGameOverWidget;

UCLASS()
class COLORDEFENSE_API AGameOverHandler : public AActor
{
    GENERATED_BODY()
    AGameOverHandler();
    virtual void BeginPlay() override;

public:
    // The function triggered when GameState reaches 0 HP
    UFUNCTION()
    void HandleGameOver();
};