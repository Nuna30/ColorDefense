#include "Widgets/WorldOverviewWidget.h"

void UWorldOverviewWidget::NativeConstruct()
{
	Super::NativeConstruct();

    // Create a RenderTarget.
	RenderTarget = NewObject<UTextureRenderTarget2D>(this);
	RenderTarget->InitCustomFormat(512, 256, EPixelFormat::PF_B8G8R8A8, false);  // ← change size here
	RenderTarget->RenderTargetFormat = RTF_RGBA8;
	RenderTarget->UpdateResourceImmediate(true);

    // Create a ChunkVisualizer.
    FIntVector StartingLocation = FIntVector(0, 0, 0);
    ChunkVisualizer = GetWorld()->SpawnActor<AChunkVisualizer>();
    ChunkVisualizer->SetStartingLocation(StartingLocation);

    // Create a capture camera.
    CaptureCamera = GetWorld()->SpawnActor<ACaptureCamera>();
    CaptureCamera->WorldOverviewCaptureCamera->TextureTarget = RenderTarget;
    CaptureCamera->SetCenterLocation(FVector(StartingLocation));
    CaptureCamera->SetActorLocation(FVector(3043, 3176, 5936));

    // Wire the RenderTarget to the image.
    FSlateBrush Brush;
    Brush.SetResourceObject(RenderTarget);
    Brush.ImageSize = FVector2D(RenderTarget->SizeX, RenderTarget->SizeY);
    WorldOverviewImage->SetBrush(Brush);
}

// ========================
// ===== Mouse Events =====
// ========================

void UWorldOverviewWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
}

void UWorldOverviewWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

FReply UWorldOverviewWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bIsDragging = true;
		bIsPanning = false;
		DragStartScreenPosition = InMouseEvent.GetScreenSpacePosition();
		HideMouseCursor();
		return FReply::Handled().CaptureMouse(TakeWidget());
	}
	else if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		bIsDragging = true;
		bIsPanning = true;
		DragStartScreenPosition = InMouseEvent.GetScreenSpacePosition();
		HideMouseCursor();
		return FReply::Handled().CaptureMouse(TakeWidget());
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UWorldOverviewWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bIsDragging &&
		(InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton ||
		 InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton))
	{
		EndDrag();
		return FReply::Handled().ReleaseMouseCapture();
	}
	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

FReply UWorldOverviewWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bIsDragging && CaptureCamera)
	{
		FVector2D Delta = InMouseEvent.GetScreenSpacePosition() - InMouseEvent.GetLastScreenSpacePosition();

		if (bIsPanning)
		{
			// 우클릭 드래그 → CenterLocation 이동 (팬)
			CaptureCamera->Pan(-Delta.X, Delta.Y);
		}
		else
		{
			// 좌클릭 드래그 → 회전 (기존)
			CaptureCamera->AddAzimuth(-Delta.X * MouseSensitivity);
			CaptureCamera->AddElevation(-Delta.Y * MouseSensitivity);
		}

		return FReply::Handled();
	}
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

FReply UWorldOverviewWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (CaptureCamera)
	{
		float WheelDelta = InMouseEvent.GetWheelDelta(); // +1 = wheel up, -1 = wheel down

		// wheel up → Zoom In 
		// wheel down → Zoom Out
		CaptureCamera->AddRadius(-WheelDelta * ZoomSensitivity);

		return FReply::Handled();
	}

	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
}

void UWorldOverviewWidget::EndDrag()
{
	bIsDragging = false;
	bIsPanning = false;

	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->SetShowMouseCursor(true);
		PC->SetMouseLocation(DragStartScreenPosition.X, DragStartScreenPosition.Y);
	}
}

// ================= //
// ===== Utils ===== //
// ================= //

void UWorldOverviewWidget::HideMouseCursor() 
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->SetShowMouseCursor(false);
	}
}