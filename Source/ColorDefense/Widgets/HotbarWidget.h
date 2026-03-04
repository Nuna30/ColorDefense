#pragma once

#include "Widgets/HotbarSlotWidget.h"
#include "Data/ActorComponents/Inventory.h"
#include "Components/HorizontalBox.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HotbarWidget.generated.h"

UCLASS()
class COLORDEFENSE_API UHotbarWidget : public UUserWidget
{
    GENERATED_BODY()
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

public: // --- Properties --- //
    const int32 SlotCount = 5;

public: // --- Synchronization --- //
    UPROPERTY()
    UInventory* PlayerInventory = nullptr;

public: // --- Features --- //
    UFUNCTION()
    void Initialize(UInventory* InInventory);

    UFUNCTION()
    void RefreshHotbar();

    UFUNCTION()
    void UpdateSelectedSlot();

public: // --- Slot system --- //
    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* SlotsContainer;

    UPROPERTY()
    TArray<UHotbarSlotWidget*> SlotWidgets;

public: // --- Events --- //
    UFUNCTION()
    void OnInventoryChanged();
};