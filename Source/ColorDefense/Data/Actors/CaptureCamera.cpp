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
	WorldOverviewCaptureCamera->CaptureScene();	
	UE_LOG(LogTemp, Warning, TEXT("Capture"));
}