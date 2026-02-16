#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ComboWidget.generated.h"

UCLASS()
class COLORDEFENSE_API UComboWidget : public UUserWidget
{
    GENERATED_BODY()
    virtual void NativeConstruct() override;

public:
    // Update the text from ColorGun
    UFUNCTION(BlueprintCallable, Category = "Combo")
    void UpdateComboText(int32 NewCombo);
    void BindDelegate();
    
protected:
    // meta = (BindWidget) looks for a TextBlock named "ComboText" in your Blueprint
    UPROPERTY(meta = (BindWidget))
    UTextBlock* ComboText;
};