#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "MoneyWidget.generated.h"

UCLASS()
class COLORDEFENSE_API UMoneyWidget : public UUserWidget
{
    GENERATED_BODY()
    virtual void NativeConstruct() override;

public:
    // Update the text from ColorGun
    UFUNCTION() // For subscribing the delegate
    void UpdateMoneyText(int32 NewMoney);

protected:
    // meta = (BindWidget) looks for a TextBlock named "MoneyText" in your Blueprint
    UPROPERTY(meta = (BindWidget))
    UTextBlock* MoneyText;
};