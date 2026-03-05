#include "HotbarWidget.h"
#include "PlayerCharacter.h"
#include "Data/Actors/ToolBox.h"

// ====================== //
// ===== Base Class ===== //
// ====================== //

void UHotbarWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Put HighLights into an array.
    HighlightArray.Empty();
    if (HighLight1) HighlightArray.Add(HighLight1);
    if (HighLight2) HighlightArray.Add(HighLight2);
    if (HighLight3) HighlightArray.Add(HighLight3);
    if (HighLight4) HighlightArray.Add(HighLight4);
    if (HighLight5) HighlightArray.Add(HighLight5);

    // Put IconImages into an array
    IconImageArray.Empty();
    if (IconImage1) IconImageArray.Add(IconImage1);
    if (IconImage2) IconImageArray.Add(IconImage2);
    if (IconImage3) IconImageArray.Add(IconImage3);
    if (IconImage4) IconImageArray.Add(IconImage4);
    if (IconImage5) IconImageArray.Add(IconImage5);

    // Get ToolBox and bind delegate.
    if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwningPlayerPawn()))
    {
        if (AToolBox* ToolBox = Cast<AToolBox>(Player->ToolBoxComponent->GetChildActor()))
        {
            ToolBoxRef = ToolBox;
            ToolBoxRef->OnToolChanged.AddDynamic(this, &UHotbarWidget::OnToolChanged);
        }
    }

    SetSelectedSlot(0);

    GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UHotbarWidget::UpdateIconImages);   // ← Show icons immediately
}

void UHotbarWidget::OnToolChanged(int32 NewIndex)
{
    SetSelectedSlot(NewIndex);
}

// ============================ //
// ===== Slot IconImages ====== //
// ============================ //

void UHotbarWidget::UpdateIconImages()
{
    for (int32 i = 0; i < IconImageArray.Num(); i++)
    {
        ATool* Tool = ToolBoxRef->ToolBox.IsValidIndex(i) ? ToolBoxRef->ToolBox[i] : nullptr;

        if (!Tool) UE_LOG(LogTemp, Warning, TEXT("Tool nullptr"));

        if (Tool && Tool->IconTexture2D)
        {
            UE_LOG(LogTemp, Warning, TEXT("Update Icon Images!"));
            IconImageArray[i]->SetBrushFromTexture(Tool->IconTexture2D);
            IconImageArray[i]->SetOpacity(1.0f);
            IconImageArray[i]->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            // Empty slot
            UE_LOG(LogTemp, Warning, TEXT("Empty slot..."));
            IconImageArray[i]->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}

// ================== //
// ===== Utils ====== //
// ================== //

void UHotbarWidget::SetSelectedSlot(int32 Index)
{
    for (int32 i = 0; i < HighlightArray.Num(); i++)
    {
        if (HighlightArray.IsValidIndex(i) && HighlightArray[i])
        {
            HighlightArray[i]->SetVisibility(i == Index ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
        }
    }
}