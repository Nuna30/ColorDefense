#include "PlayerCharacter.h"
#include "Data/Actors/ToolBox.h"
#include "Widgets/ColorDefenseHUD.h"

APlayerCharacter::APlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // FPS Camera
    FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
    FPSCamera->SetupAttachment(RootComponent);
    FPSCamera->bUsePawnControlRotation = true; // IMPORTANT: Follow mouse look

    // Invisible Arm
    InvisibleArm = CreateDefaultSubobject<USceneComponent>(TEXT("InvisibleArm"));
    InvisibleArm->SetupAttachment(FPSCamera);

    // Tool Box
    ToolBoxComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("ToolBoxComponent"));
    ToolBoxComponent->SetupAttachment(InvisibleArm);
}

void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Reset Controller
    // Switch back to Game Only
    // Hide the mouse
    APlayerController* PC = Cast<APlayerController>(GetController());
    FInputModeGameOnly InputModeData;
    PC->SetInputMode(InputModeData);
    PC->bShowMouseCursor = false;

    // Add Enhanced Input Mapping Context
    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
    Subsystem->AddMappingContext(DefaultMappingContext, 0);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up the Player Input Component.
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APlayerCharacter::LookRight);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Jump);

    // Set up the ToolBox Input Component.
    AToolBox* ToolBox = Cast<AToolBox>(ToolBoxComponent->GetChildActor());
    ToolBox->BindActions(PlayerInputComponent, this);
    ToolBox->BindCoreRemoverActionsEnhanced(this);

    // Set up the enhanced input component.
    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Shopping
        EnhancedInputComponent->BindAction(ToggleShopAction, ETriggerEvent::Started, this, &APlayerCharacter::ToggleShop);
    }
}

void APlayerCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector(), AxisValue);
}

void APlayerCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

void APlayerCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector(), AxisValue);
}

void APlayerCharacter::LookRight(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void APlayerCharacter::Jump()
{
	ACharacter::Jump();
}

// --- Shopping --- //

void APlayerCharacter::ToggleShop()
{
    // The Character no longer handles "how" to open the shop.
    // It just tells the HUD: "The player wants to toggle the UI."
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (AColorDefenseHUD* HUD = PC->GetHUD<AColorDefenseHUD>())
        {
            // We will create this function in the HUD next
            HUD->ToggleMainHUD(); 
        }
    }
}