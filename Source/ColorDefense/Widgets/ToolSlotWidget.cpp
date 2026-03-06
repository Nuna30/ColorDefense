#include "ToolSlotWidget.h"
#include "Widgets/HotbarWidget.h"
#include "Data/Actors/ToolBox.h"
#include "Tools/Tool.h"
#include "Blueprint/DragDropOperation.h" 

// ======================= //
// ===== Drag & Drop ===== //
// ======================= //

FReply UToolSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        return FReply::Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
    }

    return FReply::Unhandled();
}

void UToolSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

    if (!OwnerHotbar || !OwnerHotbar->ToolBoxRef) return;

    UDragDropOperation* Operation = NewObject<UDragDropOperation>();
    Operation->Payload = this;
    Operation->Pivot = EDragPivot::MouseDown;

    // UE 5.6+ safe drag preview (NewObject is the recommended way)
    UImage* DragIcon = NewObject<UImage>(this);
    if (DragIcon)
    {
        if (IconImage && IconImage->GetBrush().GetResourceObject())
        {
            DragIcon->SetBrush(IconImage->GetBrush());
        }
        Operation->DefaultDragVisual = DragIcon;
    }
    else
    {
        Operation->DefaultDragVisual = this;
    }

    OutOperation = Operation;
}

bool UToolSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    if (!InOperation || !OwnerHotbar || !OwnerHotbar->ToolBoxRef) return false;

    if (UToolSlotWidget* SourceSlot = Cast<UToolSlotWidget>(InOperation->Payload))
    {
        if (SourceSlot != this && SourceSlot->OwnerHotbar == OwnerHotbar)
        {
            OwnerHotbar->SwapSlots(SourceSlot->SlotIndex, this->SlotIndex);
            return true;
        }
    }
    return false;
}

// ================= //
// ===== Utils ===== //
// ================= //

void UToolSlotWidget::UpdateSlot(ATool* Tool)
{
    if (Tool && Tool->IconTexture2D)
    {
        IconImage->SetBrushFromTexture(Tool->IconTexture2D);
        IconImage->SetOpacity(1.0f);
        IconImage->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        IconImage->SetVisibility(ESlateVisibility::Hidden);
    }
}