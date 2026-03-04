#pragma once

#include "Blueprint/UserWidget.h"

#include "CoreMinimal.h"
#include "Data/ActorComponents/Inventory.h"
#include "HotbarSlotWidget.generated.h"

UCLASS()
class COLORDEFENSE_API UHotbarSlotWidget : public UUserWidget
{
    GENERATED_BODY()

public: // --- Synchronization --- //
    UPROPERTY(meta = (BindWidget))
    class UImage* ItemIcon;

    UPROPERTY(meta = (BindWidget))
    class UBorder* HighlightBorder;

    UPROPERTY()
    UInventory* OwningInventory = nullptr;

public: // --- Properties --- //
    int32 SlotIndex = -1;

public: // --- Features --- //
    void Initialize(UInventory* InInventory, int32 InIndex);
    void Refresh();
    void SetIsSelected(bool bIsSelected);

public: // --- UI Interaction --- //
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};