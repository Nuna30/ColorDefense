#include "HotbarSlotWidget.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Blueprint/DragDropOperation.h"

// ==================== //
// ===== Features ===== //
// ==================== //

void UHotbarSlotWidget::Initialize(UInventory* InInventory, int32 InIndex)
{
    OwningInventory = InInventory;
    SlotIndex = InIndex;
    Refresh();
}

void UHotbarSlotWidget::Refresh()
{
    // This is not what I had in mind.

    // const FInventoryItem& Item = OwningInventory->GetItemRef(SlotIndex);

    // if (Item.Icon && Item.Quantity > 0)
    // {
    //     ItemIcon->SetBrushFromTexture(Item.Icon);
    //     ItemIcon->SetVisibility(ESlateVisibility::Visible);
    // }
    // else
    // {
    //     ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
    // }
}

void UHotbarSlotWidget::SetIsSelected(bool bIsSelected)
{
    HighlightBorder->SetBrushColor(bIsSelected ? FLinearColor::White : FLinearColor(0.1f, 0.1f, 0.1f, 0.8f));
}

// ========================== //
// ===== UI Interaction ===== //
// ========================== //

FReply UHotbarSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        return FReply::Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
    }
    return FReply::Unhandled();
}

void UHotbarSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
    if (!OwningInventory || SlotIndex < 0) return;

    UDragDropOperation* Operation = NewObject<UDragDropOperation>();
    Operation->Payload = this;                     // Pass the source slot itself
    Operation->DefaultDragVisual = ItemIcon;       // Shows the icon while dragging

    OutOperation = Operation;
}

bool UHotbarSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    if (!OwningInventory || !InOperation) return false;

    UHotbarSlotWidget* SourceSlot = Cast<UHotbarSlotWidget>(InOperation->Payload);
    if (SourceSlot && SourceSlot != this && SourceSlot->OwningInventory == OwningInventory)
    {
        OwningInventory->SwapSlots(SourceSlot->SlotIndex, SlotIndex);
        return true;
    }
    return false;
}