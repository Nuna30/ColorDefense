#pragma once

#include "Components/ProgressBar.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HPBarWidget.generated.h"

UCLASS()
class COLORDEFENSE_API UHPBarWidget : public UUserWidget
{
    GENERATED_BODY()
    virtual void NativeConstruct() override;

public: // --- Progress Bar --- //
	UPROPERTY(meta = (BindWidget))
	UProgressBar* ProgressBarHP;

	UFUNCTION(BlueprintCallable, Category = "HP Bar")
	void UpdateHealth(int32 CurrentHP, int32 MaxHP);

	UFUNCTION(BlueprintCallable, Category = "HP Bar")
	void SetBarColor(FLinearColor NewColor);
};