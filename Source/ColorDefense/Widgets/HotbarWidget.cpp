#include "HotbarWidget.h"

void UHotbarWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UHotbarWidget::NativeDestruct()
{
    PlayerInventory->OnInventoryChanged.RemoveDynamic(this, &UHotbarWidget::OnInventoryChanged);

    Super::NativeDestruct();
}

void UHotbarWidget::Initialize(UInventory* InInventory)
{
    PlayerInventory = InInventory;

    // Reset the slots.
    SlotsContainer->ClearChildren();
    SlotWidgets.Empty();

    // Create 5 slots.
    for (int32 i = 0; i < SlotCount; ++i)
    {
        UHotbarSlotWidget* NewSlot = CreateWidget<UHotbarSlotWidget>(this, UHotbarSlotWidget::StaticClass());
        if (NewSlot)
        {
            NewSlot->Initialize(PlayerInventory, i);
            SlotsContainer->AddChild(NewSlot);
            SlotWidgets.Add(NewSlot);
        }
    }

    PlayerInventory->OnInventoryChanged.AddDynamic(this, &UHotbarWidget::OnInventoryChanged);
    RefreshHotbar();
}

void UHotbarWidget::OnInventoryChanged()
{
    RefreshHotbar();
}

void UHotbarWidget::RefreshHotbar()
{
    for (UHotbarSlotWidget* SlotWidget : SlotWidgets)
    {
        if (SlotWidget) SlotWidget->Refresh();
    }
    UpdateSelectedSlot();
}

void UHotbarWidget::UpdateSelectedSlot()
{
    if (!PlayerInventory) return;

    for (int32 i = 0; i < SlotWidgets.Num(); ++i)
    {
        if (SlotWidgets[i])
        {
            SlotWidgets[i]->SetIsSelected(i == PlayerInventory->HotbarSelectedIndex);
        }
    }
}