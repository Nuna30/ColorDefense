#include "MoneyWidget.h"
#include "ColorDefenseGameState.h"

void UMoneyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Get the GameState
    if (AColorDefenseGameState* GS = GetWorld()->GetGameState<AColorDefenseGameState>())
    {
        // Bind UpdateMoneyText function to the delegate
        GS->OnMoneyChanged.AddDynamic(this, &UMoneyWidget::UpdateMoneyText);
    }
}

void UMoneyWidget::UpdateMoneyText(int32 NewMoney)
{
    MoneyText->SetText(FText::AsNumber(NewMoney));
}