#include "ComboWidget.h"
#include "TimerManager.h"
#include "GameStates/ColorDefenseGameState.h"

void UComboWidget::NativeConstruct()
{
    Super::NativeConstruct();

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle, 
        this, 
        &UComboWidget::BindDelegate, 
        0.1f, 
        false // false means it won't loop
    );
}

void UComboWidget::UpdateComboText(int32 NewCombo)
{
    if (NewCombo <= 0)
    {
        ComboText->SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        ComboText->SetVisibility(ESlateVisibility::Visible);
        ComboText->SetText(FText::AsNumber(NewCombo));
    }
}

void UComboWidget::BindDelegate()
{
    // Bind the function to the delegate.
    if (AColorDefenseGameState* GS = Cast<AColorDefenseGameState>(GetWorld()->GetGameState()))
    {
        GS->OnComboChanged.AddDynamic(this, &UComboWidget::UpdateComboText);
        UpdateComboText(GS->GetCurrentCombo());
    }
}