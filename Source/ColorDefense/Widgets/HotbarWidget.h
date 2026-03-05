#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "HotbarWidget.generated.h"

UCLASS()
class COLORDEFENSE_API UHotbarWidget : public UUserWidget
{
    GENERATED_BODY()
    virtual void NativeConstruct() override;

public: // --- Properties --- //
    UPROPERTY()
    class AToolBox* ToolBoxRef;

public: // --- Highlighting --- //
    UPROPERTY(meta = (BindWidget))
    UImage* HighLight1;

    UPROPERTY(meta = (BindWidget))
    UImage* HighLight2;

    UPROPERTY(meta = (BindWidget))
    UImage* HighLight3;

    UPROPERTY(meta = (BindWidget))
    UImage* HighLight4;

    UPROPERTY(meta = (BindWidget))
    UImage* HighLight5;

    UPROPERTY()
    TArray<UImage*> HighlightArray;

    UFUNCTION()
    void OnToolChanged(int32 NewIndex);

public: // --- Slot IconImages --- //
    UPROPERTY(meta = (BindWidget))
    UImage* IconImage1;

    UPROPERTY(meta = (BindWidget))
    UImage* IconImage2;

    UPROPERTY(meta = (BindWidget))
    UImage* IconImage3;

    UPROPERTY(meta = (BindWidget))
    UImage* IconImage4;

    UPROPERTY(meta = (BindWidget))
    UImage* IconImage5;

    UPROPERTY()
    TArray<UImage*> IconImageArray;

    UFUNCTION()
    void UpdateIconImages();

public: // --- Utils --- //
    void SetSelectedSlot(int32 Index);
};