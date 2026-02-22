// GameOverWidget.cpp
#include "GameOverWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGameOverWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (GoMainMenuButton)
    {
        GoMainMenuButton->OnClicked.AddDynamic(this, &UGameOverWidget::OnGoMainMenuClicked);
    }
}

void UGameOverWidget::OnGoMainMenuClicked()
{
    UGameplayStatics::OpenLevel(this, FName("MainMenu"));
}