#include "Data/Actors/CaptureCamera.h"

// ========================= //
// ===== Base Function ===== //
// ========================= //

ACaptureCamera::ACaptureCamera()
{
	PrimaryActorTick.bCanEverTick = false;   // We use timer instead

	// Create a capture camera.
	WorldOverviewCaptureCamera = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("WorldOverviewCaptureCamera"));
	WorldOverviewCaptureCamera->SetupAttachment(RootComponent);
	WorldOverviewCaptureCamera->bCaptureEveryFrame = false;
	WorldOverviewCaptureCamera->bCaptureOnMovement = false;
	WorldOverviewCaptureCamera->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
}

void ACaptureCamera::BeginPlay()
{
	Super::BeginPlay();

	// Start controlled updates
	GetWorldTimerManager().SetTimer(CaptureCameraTimer, this, &ACaptureCamera::UpdateWorldOverviewCapture, UpdateInterval, true);
}

// ==================== //
// ===== Features ===== //
// ==================== //

void ACaptureCamera::UpdateWorldOverviewCapture()
{
	UpdateSphericalTrajectory(UpdateInterval);
	WorldOverviewCaptureCamera->CaptureScene();	
}

void ACaptureCamera::UpdateSphericalTrajectory(float DeltaTime)
{
    if (Radius <= 0.0f) return;

    // 1. 방위각 회전 (360도 순환)
    CurrentAzimuth += OrbitSpeed * DeltaTime;
    CurrentAzimuth = FMath::Fmod(CurrentAzimuth, 360.0f);

    // 2. 구면 좌표 → 카테시안 좌표 변환
    float AzRad = FMath::DegreesToRadians(CurrentAzimuth);
    float ElRad = FMath::DegreesToRadians(ElevationAngle);

    FVector Offset(
        Radius * FMath::Cos(AzRad) * FMath::Cos(ElRad),   // X
        Radius * FMath::Sin(AzRad) * FMath::Cos(ElRad),   // Y
        Radius * FMath::Sin(ElRad)                        // Z (위로)
    );

    FVector NewLocation = CenterLocation + Offset;

    // 3. 위치 이동 + 항상 중심을 바라보게 회전
    SetActorLocation(NewLocation);
    SetActorRotation((CenterLocation - NewLocation).Rotation());
}

void ACaptureCamera::SetCenterLocation(FVector NewCenterLocation)
{
	CenterLocation = FVector(NewCenterLocation);
}