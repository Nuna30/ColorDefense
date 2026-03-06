#pragma once

#include "Components/Image.h"
#include "PlayerCharacter.h"
#include "Data/Actors/ToolBox.h"
#include "Widgets/ToolSlotWidget.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

public: // --- IconImages --- //
    // UPROPERTY(meta = (BindWidget))
    // UImage* IconImage1;

    // UPROPERTY(meta = (BindWidget))
    // UImage* IconImage2;

    // UPROPERTY(meta = (BindWidget))
    // UImage* IconImage3;

    // UPROPERTY(meta = (BindWidget))
    // UImage* IconImage4;

    // UPROPERTY(meta = (BindWidget))
    // UImage* IconImage5;

    // UPROPERTY()
    // TArray<UImage*> IconImageArray;

public: // --- Slots --- //
    UPROPERTY(meta = (BindWidget))
    UToolSlotWidget* SlotWidget1;

    UPROPERTY(meta = (BindWidget))
    UToolSlotWidget* SlotWidget2;

    UPROPERTY(meta = (BindWidget))
    UToolSlotWidget* SlotWidget3;

    UPROPERTY(meta = (BindWidget))
    UToolSlotWidget* SlotWidget4;

    UPROPERTY(meta = (BindWidget))
    UToolSlotWidget* SlotWidget5;

    UPROPERTY()
    TArray<UToolSlotWidget*> SlotWidgetArray;

    UFUNCTION()
    void UpdateAllSlots();

    void SwapSlots(int32 FromIndex, int32 ToIndex);

public: // --- Utils --- //
    void SetSelectedSlot(int32 Index);
};