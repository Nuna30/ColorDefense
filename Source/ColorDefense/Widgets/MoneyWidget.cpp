#include "MoneyWidget.h"
#include "ColorDefenseGameState.h"

void UMoneyWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UMoneyWidget::UpdateMoneyText(int32 NewMoney)
{
    MoneyText->SetText(FText::AsNumber(NewMoney));
}