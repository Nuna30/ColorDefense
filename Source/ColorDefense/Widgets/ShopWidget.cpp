// ShopWidget.cpp
#include "ShopWidget.h"
#include "CreepPathWidget.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"

void UShopWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Make connection with CreepPath Widget
    if (APlayerController* PC = GetOwningPlayer())
    {
        if (CreepPathWidgetClass && !CreepPathWidgetInstance)
        {
            CreepPathWidgetInstance = CreateWidget<UCreepPathWidget>(PC, CreepPathWidgetClass);
            if (CreepPathWidgetInstance)
            {
                // Pass THIS widget to the instance instead of letting it create a new one
                CreepPathWidgetInstance->ShopWidgetInstance = this; 
                
                CreepPathWidgetInstance->AddToViewport(1);
                CreepPathWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
            }
        }
    }

    // Binding buttons.
    if (CreepPathButton)
    {
        // Remove first to ensure we don't bind twice
        CreepPathButton->OnClicked.RemoveDynamic(this, &UShopWidget::OnCreepPathClicked);
        CreepPathButton->OnClicked.AddDynamic(this, &UShopWidget::OnCreepPathClicked);
    }

    if (QuitButton)
    {
        QuitButton->OnClicked.RemoveDynamic(this, &UShopWidget::OnQuitClicked);
        QuitButton->OnClicked.AddDynamic(this, &UShopWidget::OnQuitClicked);
    }
}

void UShopWidget::OnCreepPathClicked()
{
    // Hide the Shop contents
    if (ShopCanvasPanel)
    {
        ShopCanvasPanel->SetVisibility(ESlateVisibility::Collapsed);
    }

    // Show the Creep Path widget
    if (CreepPathWidgetInstance)
    {
        CreepPathWidgetInstance->SetVisibility(ESlateVisibility::Visible);

        if (APlayerController* PC = GetOwningPlayer())
        {
            FInputModeGameAndUI InputMode;
            // Focus the widget instance directly, not the Slate widget
            InputMode.SetWidgetToFocus(CreepPathWidgetInstance->GetCachedWidget()); 
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = true;
        }
    }
}

void UShopWidget::OnQuitClicked()
{
    
}