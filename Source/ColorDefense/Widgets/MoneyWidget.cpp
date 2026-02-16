#include "MoneyWidget.h"
#include "TimerManager.h"
#include "ColorDefenseGameState.h"

void UMoneyWidget::NativeConstruct()
{
    Super::NativeConstruct();

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle, 
        this, 
        &UMoneyWidget::BindDelegate, 
        0.1f, 
        false // false means it won't loop
    );
}

void UMoneyWidget::UpdateMoneyText(int32 NewMoney)
{
    MoneyText->SetText(FText::AsNumber(NewMoney));
}

void UMoneyWidget::BindDelegate()
{
    // Bind the function to the delegate.
    if (AColorDefenseGameState* GS = Cast<AColorDefenseGameState>(GetWorld()->GetGameState()))
    {
        GS->OnMoneyChanged.AddDynamic(this, &UMoneyWidget::UpdateMoneyText);
        UpdateMoneyText(GS->GetCurrentMoney());
    }
}