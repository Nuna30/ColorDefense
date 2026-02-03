#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ColorDefenseGameState.generated.h"

UCLASS()
class COLORDEFENSE_API AColorDefenseGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Economy")
    int32 CurrentMoney = 0;

public:
    int32 GetCurrentMoney() const;

    // Adds money and broadcasts the change
    void AddMoney(int32 Amount);

    // Returns true if purchase is successful
    bool TrySpendMoney(int32 Cost);
};