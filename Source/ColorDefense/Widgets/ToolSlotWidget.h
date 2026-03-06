#pragma once

#include "Components/Image.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ToolSlotWidget.generated.h"

UCLASS()
class COLORDEFENSE_API UToolSlotWidget : public UUserWidget
{
    GENERATED_BODY()

public: // --- Properties --- //
    UPROPERTY(meta = (BindWidget))
    UImage* IconImage;

    int32 SlotIndex = -1; // Decided by hotbar widget

    class UHotbarWidget* OwnerHotbar = nullptr;   // will be set by HotbarWidget

public: // --- Drag & Drop --- //
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
    virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public: // --- Utils --- //
    void UpdateSlot(class ATool* Tool);   // refreshes icon + visibility
};