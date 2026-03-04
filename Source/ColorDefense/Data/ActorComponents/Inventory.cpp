#include "Data/ActorComponents/Inventory.h"

// ====================== //
// ===== Base Class ===== //
// ====================== //

UInventory::UInventory()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UInventory::BeginPlay()
{
    Super::BeginPlay();
    if (Items.Num() == 0)
        Items.SetNum(5); // 5 hotbar slots (empty by default)
}

void UInventory::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// ==================== //
// ===== Features ===== //
// ==================== //

bool UInventory::AddItem(FInventoryItem NewItem)
{
    // TODO: your full stacking logic here (for now just add to first empty slot)
    for (int32 i = 0; i < Items.Num(); ++i)
    {
        if (Items[i].Quantity == 0)
        {
            Items[i] = NewItem;
            OnInventoryChanged.Broadcast();
            return true;
        }
    }
    return false;
}

void UInventory::RemoveItem(int32 SlotIndex, int32 Amount)
{
	// TODO: your logic
}

void UInventory::SwapSlots(int32 SlotA, int32 SlotB)
{
	Items.Swap(SlotA, SlotB);
	OnInventoryChanged.Broadcast();
}

FInventoryItem& UInventory::GetItemRef(int32 Index)
{
    return Items[Index];
}