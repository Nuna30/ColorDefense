#pragma once

#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

UCLASS()
class COLORDEFENSE_API UGameOverWidget : public UUserWidget
{
    GENERATED_BODY()
    virtual void NativeConstruct() override;
    
public:
    UPROPERTY(meta = (BindWidget))
    class UButton* GoMainMenuButton;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* GameOverText;

public:
    UFUNCTION()
    void OnGoMainMenuClicked();
};