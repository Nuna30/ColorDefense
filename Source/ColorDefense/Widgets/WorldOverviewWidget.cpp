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
	// 필요시 hover 효과 추가 가능
}

void UWorldOverviewWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (bIsDragging) EndDrag();
}

FReply UWorldOverviewWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && CaptureCamera)
	{
		bIsDragging = true;
		DragStartScreenPosition = InMouseEvent.GetScreenSpacePosition();

		if (APlayerController* PC = GetOwningPlayer())
		{
			PC->SetShowMouseCursor(false);
		}

		return FReply::Handled().CaptureMouse(TakeWidget());
	}
	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UWorldOverviewWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && bIsDragging)
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

		CaptureCamera->AddAzimuth(-Delta.X * MouseSensitivity);   // 좌우 반전
		CaptureCamera->AddElevation(Delta.Y * MouseSensitivity);  // 상하 (필요시 - 붙여서 반전)

		return FReply::Handled();
	}
	return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

void UWorldOverviewWidget::EndDrag()
{
	bIsDragging = false;

	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->SetShowMouseCursor(true);
		PC->SetMouseLocation(DragStartScreenPosition.X, DragStartScreenPosition.Y);
	}
}