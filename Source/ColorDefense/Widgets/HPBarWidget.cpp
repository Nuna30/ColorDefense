#include "HPBarWidget.h"

void UHPBarWidget::NativeConstruct()
{
    Super::NativeConstruct();

    SetVisibility(ESlateVisibility::Hidden);
}

void UHPBarWidget::UpdateHealth(int32 CurrentHP, int32 MaxHP)
{
    if (CurrentHP < MaxHP) SetVisibility(ESlateVisibility::Visible);

    float Percent = MaxHP > 0 ? (float)CurrentHP / (float)MaxHP : 0.0f;
    ProgressBarHP->SetPercent(Percent);
}

void UHPBarWidget::SetBarColor(FLinearColor NewColor)
{
    ProgressBarHP->SetFillColorAndOpacity(NewColor);
}