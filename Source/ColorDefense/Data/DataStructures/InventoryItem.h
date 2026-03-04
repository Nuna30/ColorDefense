#pragma once

#include "Data/Enums/Name.h"

#include "CoreMinimal.h"
#include "InventoryItem.generated.h"

USTRUCT()
struct FInventoryItem
{
    GENERATED_BODY()

    UPROPERTY();
    EItemName ItemName;

    UPROPERTY(EditAnywhere, Category = "Setting")
    int32 Quantity = 0;

    UPROPERTY(EditAnywhere, Category = "Setting")
    int32 MaxStack = 64;

    UPROPERTY(EditAnywhere, Category = "Setting")
    UTexture2D* Icon = nullptr;

    UPROPERTY(EditAnywhere, Category = "Setting")
    int32 Durability = 0;
};