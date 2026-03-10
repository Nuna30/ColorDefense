// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CaptureCamera.generated.h"

UCLASS()
class COLORDEFENSE_API ACaptureCamera : public AActor
{
	GENERATED_BODY()
	ACaptureCamera();
	virtual void BeginPlay() override;

public:	// --- Properties --- //

	UPROPERTY(EditAnywhere, Category = "Setting")
	float UpdateInterval = 0.08f;

public: // --- Components --- //
	UPROPERTY(VisibleAnywhere, Category = "Capture")
	USceneCaptureComponent2D* WorldOverviewCaptureCamera;

public: // --- FPS --- //
	FTimerHandle CaptureCameraTimer;

public: // --- Features --- //
	void UpdateWorldOverviewCapture();
};
