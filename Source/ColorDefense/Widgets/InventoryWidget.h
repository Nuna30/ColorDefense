#pragma once

#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

UCLASS()
class COLORDEFENSE_API UInventoryWidget : public UUserWidget
{
    GENERATED_BODY()
    virtual void NativeConstruct() override;

public:
};