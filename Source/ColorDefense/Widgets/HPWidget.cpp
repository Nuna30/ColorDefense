#include "HPWidget.h"
#include "TimerManager.h"
#include "Components/TextBlock.h"
#include "GameStates/ColorDefenseGameState.h"

void UHPWidget::NativeConstruct()
{
    Super::NativeConstruct();

    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(
        TimerHandle, 
        this, 
        &UHPWidget::BindDelegate, 
        0.1f, 
        false // false means it won't loop
    );
}

void UHPWidget::UpdateHPText(int32 NewHP)
{
    HPText->SetText(FText::AsNumber(NewHP));
}

void UHPWidget::BindDelegate()
{
    // Bind the function to the delegate.
    if (AColorDefenseGameState* GS = Cast<AColorDefenseGameState>(GetWorld()->GetGameState()))
    {
        GS->OnHPChanged.AddDynamic(this, &UHPWidget::UpdateHPText);
        
        // Initialize HP
        UpdateHPText(GS->GetCurrentHP());
    }
}