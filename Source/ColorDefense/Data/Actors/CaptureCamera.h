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

	UPROPERTY(EditAnywhere, Category = "Settings")
	float Radius = 6200.0f;               // 카메라 거리 (기본값은 월드 크기에 맞춤)

	UPROPERTY(EditAnywhere, Category = "Settings")
	float OrbitSpeed = 25.0f;             // 회전 속도 (도/초)

	UPROPERTY(EditAnywhere, Category = "Settings")
	float ElevationAngle = 65.0f;         // 고도 각도 (65°가 월드 오버뷰에 가장 자연스러움)

	UPROPERTY(EditAnywhere, Category = "Settings")
	float PanSensitivity = 1.0f;          // ← 우클릭 팬 속도

	FVector CenterLocation;
	float CurrentAzimuth = 0.0f;


public: // --- Components --- //
	UPROPERTY(VisibleAnywhere, Category = "Capture")
	USceneCaptureComponent2D* WorldOverviewCaptureCamera;

public: // --- FPS --- //
	FTimerHandle CaptureCameraTimer;

public: // --- Features --- //
	void UpdateWorldOverviewCapture();
	void SetCenterLocation(FVector NewCenterLocation);
	void UpdateSphericalTrajectory(float DeltaTime);
	void ApplySphericalPosition();

public: // --- Manual Control --- //
	UFUNCTION()
	void AddAzimuth(float Delta);

	UFUNCTION()
	void AddElevation(float Delta);

	UFUNCTION()
	void AddRadius(float Delta);

	UFUNCTION()
	void Pan(float DeltaX, float DeltaY);
};
