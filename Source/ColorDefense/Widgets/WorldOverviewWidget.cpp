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
    ChunkVisualizer = GetWorld()->SpawnActor<AChunkVisualizer>();
    ChunkVisualizer->SetStartingLocation(FIntVector(0, 0, 0));

    // Create a capture camera.
    CaptureCamera = GetWorld()->SpawnActor<ACaptureCamera>();
    CaptureCamera->WorldOverviewCaptureCamera->TextureTarget = RenderTarget;
    CaptureCamera->SetActorLocation(FVector(3043, 3176, 5936));

    내일 컨트롤 부분 만들면 될 듯 ㅇㅇ


    // Wire the RenderTarget to the image.
    FSlateBrush Brush;
    Brush.SetResourceObject(RenderTarget);
    Brush.ImageSize = FVector2D(RenderTarget->SizeX, RenderTarget->SizeY);
    WorldOverviewImage->SetBrush(Brush);
}