#include "ColorDefenseGameState.h"
#include "TimerManager.h" // Required for timers

void AColorDefenseGameState::BeginPlay()
{
    Super::BeginPlay();
    
    // Set a timer to run 'BroadcastInitialMoney' after 0.1 seconds  so all widgets catch it even if they constructed slightly late.
    // Check my note on Feb 13th.
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(
        TimerHandle, 
        this, 
        &AColorDefenseGameState::BroadcastInitialMoney, 
        0.1f, 
        false // false means it won't loop
    );
}

void AColorDefenseGameState::BroadcastInitialMoney()
{
    OnMoneyChanged.Broadcast(CurrentMoney);
}

int32 AColorDefenseGameState::GetCurrentMoney() const
{
    return CurrentMoney; 
}

void AColorDefenseGameState::AddMoney(int32 Amount)
{
    CurrentMoney += Amount;

    OnMoneyChanged.Broadcast(CurrentMoney);
}

bool AColorDefenseGameState::TrySpendMoney(int32 Cost)
{
    if (CurrentMoney >= Cost)
    {
        CurrentMoney -= Cost;
        OnMoneyChanged.Broadcast(CurrentMoney);
        return true;
    }
    return false;
}