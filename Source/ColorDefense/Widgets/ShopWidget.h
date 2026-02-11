#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "ShopWidget.generated.h"

UCLASS()
class COLORDEFENSE_API UShopWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UPROPERTY(meta = (BindWidget))
    class UButton* CreepPathButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* QuitButton;

    UPROPERTY(meta = (BindWidget))
    class UCanvasPanel* ShopCanvasPanel;

    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnCreepPathClicked();

    UFUNCTION()
    void OnQuitClicked();
};