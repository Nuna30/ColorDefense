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