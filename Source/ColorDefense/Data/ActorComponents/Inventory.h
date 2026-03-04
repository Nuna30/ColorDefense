#pragma once

#include "Data/DataStructures/InventoryItem.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory.generated.h"

UCLASS()
class COLORDEFENSE_API UInventory : public UActorComponent
{
    GENERATED_BODY()
	UInventory();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public: // --- Properties --- //
    UPROPERTY()
    TArray<FInventoryItem> Items;

    UPROPERTY()
    int32 HotbarSelectedIndex = 0;

public: // --- Features --- //
    UFUNCTION()
    bool AddItem(FInventoryItem NewItem);  

    UFUNCTION()
    void RemoveItem(int32 SlotIndex, int32 Amount);

    UFUNCTION()
    void SwapSlots(int32 SlotA, int32 SlotB);

    UFUNCTION()
    FInventoryItem& GetItemRef(int32 Index); 

public: // --- Events --- //
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);
    UPROPERTY(BlueprintAssignable) FOnInventoryChanged OnInventoryChanged;
};