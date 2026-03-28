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

	ApplySphericalPosition();
}

void ACaptureCamera::ApplySphericalPosition()
{
	if (Radius <= 0.0f) return;

	float AzRad = FMath::DegreesToRadians(CurrentAzimuth);
	float ElRad = FMath::DegreesToRadians(ElevationAngle);

	FVector Offset(
		Radius * FMath::Cos(AzRad) * FMath::Cos(ElRad),
		Radius * FMath::Sin(AzRad) * FMath::Cos(ElRad),
		Radius * FMath::Sin(ElRad)
	);

	FVector NewLocation = CenterLocation + Offset;

	SetActorLocation(NewLocation);
	SetActorRotation((CenterLocation - NewLocation).Rotation());
}

void ACaptureCamera::SetCenterLocation(FVector NewCenterLocation)
{
	CenterLocation = FVector(NewCenterLocation);
}

// ========================== //
// ===== Manual Control ===== //
// ========================== //

void ACaptureCamera::AddAzimuth(float Delta)
{
	CurrentAzimuth += Delta;
	CurrentAzimuth = FMath::Fmod(CurrentAzimuth, 360.0f);
	ApplySphericalPosition();
}

void ACaptureCamera::AddElevation(float Delta)
{
	ElevationAngle = FMath::Clamp(ElevationAngle + Delta, 15.0f, 85.0f);
	ApplySphericalPosition();
}

void ACaptureCamera::AddRadius(float Delta)
{
	Radius += Delta;
	ApplySphericalPosition();
}

void ACaptureCamera::Pan(float DeltaX, float DeltaY)
{
	if (Radius <= 0.0f) return;

	// 현재 Azimuth에 맞춰 XY 평면 방향 계산 (수평 팬만)
	float AzRad = FMath::DegreesToRadians(CurrentAzimuth);

	FVector RightVector(-FMath::Sin(AzRad),  FMath::Cos(AzRad), 0.0f);   // 화면 가로
	FVector ForwardVector( FMath::Cos(AzRad), FMath::Sin(AzRad), 0.0f);  // 화면 세로

	float Scale = Radius * 0.005f * PanSensitivity;   // 거리가 멀수록 빠르게 이동

	FVector Movement = RightVector * DeltaX + ForwardVector * DeltaY;
	Movement *= Scale;

	CenterLocation += Movement;
	ApplySphericalPosition();
}