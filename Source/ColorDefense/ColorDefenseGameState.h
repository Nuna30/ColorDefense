#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ColorDefenseGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoneyChanged, int32, NewMoney);

UCLASS()
class COLORDEFENSE_API AColorDefenseGameState : public AGameStateBase
{
    GENERATED_BODY()

    virtual void BeginPlay() override;

public: // --- Money --- //

    UPROPERTY(EditAnywhere, Category = "Economy")
    int32 CurrentMoney = 1000;

    UPROPERTY()
    FOnMoneyChanged OnMoneyChanged;

    void BroadcastInitialMoney(); // Run only once when the game starts.

    int32 GetCurrentMoney() const;

    // Adds money and broadcasts the change
    void AddMoney(int32 Amount);

    // Returns true if purchase is successful
    bool TrySpendMoney(int32 Cost);
};