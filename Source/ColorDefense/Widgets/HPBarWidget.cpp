#include "HPBarWidget.h"

void UHPBarWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UHPBarWidget::UpdateHealth(int32 CurrentHP, int32 MaxHP)
{
    UE_LOG(LogTemp, Warning, TEXT("Update Health"));
    float Percent = MaxHP > 0 ? (float)CurrentHP / (float)MaxHP : 0.0f;
    ProgressBarHP->SetPercent(Percent);
}

void UHPBarWidget::SetBarColor(FLinearColor NewColor)
{
	if (ProgressBarHP)
	{
		ProgressBarHP->SetFillColorAndOpacity(NewColor);
	}
}