#include "ComboWidget.h"

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