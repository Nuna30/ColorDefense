#pragma once

#include "Components/Image.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Data/Actors/ChunkVisualizer.h"
#include "Data/Actors/CaptureCamera.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorldOverviewWidget.generated.h"

UCLASS()
class COLORDEFENSE_API UWorldOverviewWidget : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;

protected: // --- UE mouse event --- //
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public: // --- mouse control --- //
	bool bIsDragging = false;
	FVector2D DragStartScreenPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction|Mouse")
	float MouseSensitivity = 0.25f;   // ← Blueprint에서 조정 가능

	void EndDrag();

public: // --- Components --- //
	UPROPERTY(meta = (BindWidget))
	UImage* WorldOverviewImage;

	UPROPERTY()
	UTextureRenderTarget2D* RenderTarget;

	UPROPERTY()
	AChunkVisualizer* ChunkVisualizer;

	UPROPERTY()
	ACaptureCamera* CaptureCamera;
};
