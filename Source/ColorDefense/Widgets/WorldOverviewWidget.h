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
