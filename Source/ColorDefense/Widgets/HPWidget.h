#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HPWidget.generated.h"

UCLASS()
class COLORDEFENSE_API UHPWidget : public UUserWidget
{
    GENERATED_BODY()
    virtual void NativeConstruct() override;

public:
    // Update the text from ColorGun
    UFUNCTION(BlueprintCallable, Category = "HP")
    void UpdateHPText(int32 NewHP);
    void BindDelegate();
    
protected:
    // meta = (BindWidget) looks for a TextBlock named "HPText" in your Blueprint
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* HPText;
};