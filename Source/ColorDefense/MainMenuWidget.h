#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MainMenuWidget.generated.h"

UCLASS()
class COLORDEFENSE_API UMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    UPROPERTY(meta = (BindWidget))
    class UButton* PlayButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* QuitButton;

    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnPlayClicked();

    UFUNCTION()
    void OnQuitClicked();
};