#include "HotbarWidget.h"

// ====================== //
// ===== Base Class ===== //
// ====================== //

void UHotbarWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Put HighLights into an array.
    HighlightArray.Empty();
    if (HighLight1) HighlightArray.Add(HighLight1);
    if (HighLight2) HighlightArray.Add(HighLight2);
    if (HighLight3) HighlightArray.Add(HighLight3);
    if (HighLight4) HighlightArray.Add(HighLight4);
    if (HighLight5) HighlightArray.Add(HighLight5);

    // Put SlotWidgets into an array
    SlotWidgetArray.Empty();
    if (SlotWidget1) SlotWidgetArray.Add(SlotWidget1);
    if (SlotWidget2) SlotWidgetArray.Add(SlotWidget2);
    if (SlotWidget3) SlotWidgetArray.Add(SlotWidget3);
    if (SlotWidget4) SlotWidgetArray.Add(SlotWidget4);
    if (SlotWidget5) SlotWidgetArray.Add(SlotWidget5);

    for (int32 i = 0; i < SlotWidgetArray.Num(); i++)
    {
        if (SlotWidgetArray[i])
        {
            SlotWidgetArray[i]->SlotIndex = i;
            SlotWidgetArray[i]->OwnerHotbar = this;   // Needed in the drag & drop system.
        }
    }

    // Get ToolBox and bind delegate.
    if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwningPlayerPawn()))
    {
        if (AToolBox* ToolBox = Cast<AToolBox>(Player->ToolBoxComponent->GetChildActor()))
        {
            ToolBoxRef = ToolBox;
            ToolBoxRef->OnToolChanged.AddDynamic(this, &UHotbarWidget::OnToolChanged);
            ToolBoxRef->OnToolHoldInfoUpdated.AddDynamic(this, &UHotbarWidget::UpdateAllSlots);
        }
    }

    SetSelectedSlot(0);

    GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UHotbarWidget::UpdateAllSlots);   // ← Show icons immediately
}

void UHotbarWidget::OnToolChanged(int32 NewIndex)
{
    SetSelectedSlot(NewIndex);
}

// ================== //
// ===== Slots ====== //
// ================== //

void UHotbarWidget::UpdateAllSlots()
{
    for (int32 i = 0; i < SlotWidgetArray.Num(); i++)
    {
        if (ToolBoxRef->HoldInfoArray.IsValidIndex(i))
        {
            if (ATool* Tool = ToolBoxRef->HoldInfoArray[i].Tool)
            {
                SlotWidgetArray[i]->UpdateSlot(Tool);
            }
        }
    }
}

void UHotbarWidget::SwapSlots(int32 FromIndex, int32 ToIndex)
{
    ToolBoxRef->SwapTools(FromIndex, ToIndex);
    UpdateAllSlots();
}

// ================= //
// ===== Utils ===== //
// ================= //

void UHotbarWidget::SetSelectedSlot(int32 Index)
{
    for (int32 i = 0; i < HighlightArray.Num(); i++)
    {
        if (HighlightArray.IsValidIndex(i) && HighlightArray[i])
        {
            HighlightArray[i]->SetVisibility(i == Index ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
        }
    }
}

