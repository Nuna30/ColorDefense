#include "MoneyWidget.h"

void UMoneyWidget::UpdateMoneyText(int32 NewMoney)
{
    MoneyText->SetText(FText::AsNumber(NewMoney));
}