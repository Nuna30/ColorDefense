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

// --- Money --- //

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

// --- Combo --- //

int32 AColorDefenseGameState::GetCurrentCombo() const
{
    return CurrentCombo; 
}

void AColorDefenseGameState::AddCombo(int32 Amount)
{
    CurrentCombo += Amount;

    OnComboChanged.Broadcast(CurrentCombo);
}

void AColorDefenseGameState::SetCombo(int32 NewCombo)
{
    CurrentCombo = NewCombo;
    OnComboChanged.Broadcast(NewCombo);
}

// --- HP --- //

int32 AColorDefenseGameState::GetCurrentHP() const
{
    return CurrentHP; 
}

void AColorDefenseGameState::AddHP(int32 Amount)
{
    CurrentHP += Amount;

    OnHPChanged.Broadcast(CurrentHP);

    if (CurrentHP <= 0)
    {
        OnHPZero.Broadcast();
    }
}

void AColorDefenseGameState::SetHP(int32 NewHP)
{
    CurrentHP = NewHP;
    OnHPChanged.Broadcast(NewHP);
}