#include "ColorDefenseGameState.h"

int32 AColorDefenseGameState::GetCurrentMoney() const
{
    return CurrentMoney; 
}

void AColorDefenseGameState::AddMoney(int32 Amount)
{
    CurrentMoney += Amount;
}

bool AColorDefenseGameState::TrySpendMoney(int32 Cost)
{
    if (CurrentMoney >= Cost)
    {
        CurrentMoney -= Cost;
        return true;
    }
    return false;
}