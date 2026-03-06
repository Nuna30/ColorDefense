#include "Data/Actors/ToolBox.h"
#include "Utils/Utils.h"
#include "PlayerCharacter.h"

// ========================== //
// ===== Base Functions ===== //
// ========================== //

AToolBox::AToolBox()
{
	PrimaryActorTick.bCanEverTick = false;

	// Scene Component
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Core Remover
    CoreRemoverComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("CoreRemoverComponent"));
    CoreRemoverComponent->SetupAttachment(RootComponent);

	// Core Remover
    RifleComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("RifleComponent"));
    RifleComponent->SetupAttachment(RootComponent);

	// Player Block
    // PlayerBlockComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("PlayerBlockComponent"));
    // PlayerBlockComponent->SetupAttachment(RootComponent);

	// // Turret
    // TurretComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("TurretComponent"));
    // TurretComponent->SetupAttachment(RootComponent);
}

void AToolBox::BeginPlay()
{
	Super::BeginPlay();

	// Initialize the tools.
    CoreRemover = Cast<ACoreRemover>(CoreRemoverComponent->GetChildActor());
    Rifle       = Cast<ARifle>(RifleComponent->GetChildActor());
	// PlayerBlock = Cast<APlayerBlock>(PlayerBlockComponent->GetChildActor());
	// Turret = Cast<ATurret>(TurretComponent->GetChildActor());

    // Make the HoldInfo.
    FHoldInfo CoreRemoverHoldInfo = FHoldInfo(CoreRemover, EPlayerState::HoldingCoreRemover);
    FHoldInfo RifleHoldInfo = FHoldInfo(Rifle, EPlayerState::HoldingRifle);

    // Put the HoldInfos in the box.
    HoldInfoArray.Add(CoreRemoverHoldInfo);
    HoldInfoArray.Add(RifleHoldInfo);
	// ToolBox.Add(PlayerBlock);
	// ToolBox.Add(Turret);

    // Initialize Tools
    for (FHoldInfo& HoldInfo : HoldInfoArray)
    {
        HoldInfo.Tool->UnEquip();
    }

	// Set up the States
    CurrentHoldInfo = CoreRemoverHoldInfo;
    CurrentHoldInfo.Tool->SwitchVisibleActorFrom(CurrentHoldInfo.Tool);
}

void AToolBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// ========================= //
// ===== Bind Mappings ===== //
// ========================= //

void AToolBox::BindActions(UInputComponent* PlayerInputComponent, APlayerCharacter* OwnerCharacter)
{
	PlayerInputComponent->BindAction(TEXT("LeftClick"),   EInputEvent::IE_Pressed,  this, &AToolBox::HandleLeftClick);
	PlayerInputComponent->BindAction(TEXT("LeftClick"),   EInputEvent::IE_Released, this, &AToolBox::HandleLeftClickReleased);
	PlayerInputComponent->BindAction(TEXT("RightClick"),  EInputEvent::IE_Pressed,  this, &AToolBox::HandleRightClick);
}

void AToolBox::BindToolActionsEnhanced(APlayerCharacter* OwnerCharacter)
{
	// Get the Enhanced Input Component from the owning PlayerCharacter
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(OwnerCharacter->InputComponent);

	// Core Remover Enhanced Input
	EnhancedInputComponent->BindAction(ChangeColorAction, ETriggerEvent::Started, this, &AToolBox::HandleChangeColor);
	EnhancedInputComponent->BindAction(SwitchToolAction, ETriggerEvent::Started, this, &AToolBox::HandleSwitchTool);
}

void AToolBox::HandleLeftClick()
{
    CurrentHoldInfo.Tool->LeftClick();
}

void AToolBox::HandleLeftClickReleased()
{
	CurrentHoldInfo.Tool->LeftClickReleased();
}

void AToolBox::HandleRightClick()
{
    CurrentHoldInfo.Tool->RightClick(); 
}

void AToolBox::HandleChangeColor(const FInputActionValue& Value)
{
    // Get a float from the input (1.0 for Key 1, 2.0 for Key 2, etc.)
    float InputValue = Value.Get<float>();
    int32 ColorIndex = InputValue;
    EColor NewColor = Utils::IndexToColor(ColorIndex);

    // Change current color.
	CurrentHoldInfo.Tool->ChangeColor(NewColor);
}

void AToolBox::HandleSwitchTool(const FInputActionValue& Value)
{
    float InputValue = Value.Get<float>();
    int32 NewToolIndex = FMath::FloorToInt(InputValue);
    NewToolIndex--;

    SwitchTools(NewToolIndex);
}

// ================= //
// ===== Utils ===== //
// ================= //

void AToolBox::SwapTools(int32 IndexA, int32 IndexB)
{
    HoldInfoArray.Swap(IndexA, IndexB);
    SwitchTools(IndexB);
    OnToolHoldInfoUpdated.Broadcast();
}

void AToolBox::SwitchTools(int32 NewToolIndex)
{
    if (!HoldInfoArray.IsValidIndex(NewToolIndex)) return;

    // New Tool
    FHoldInfo NewHoldInfo = HoldInfoArray[NewToolIndex];

    // Switch tools.
    NewHoldInfo.Tool->SwitchVisibleActorFrom(CurrentHoldInfo.Tool);
    CurrentHoldInfo = NewHoldInfo;

    // Broadcast ToolChanged.
    OnToolChanged.Broadcast(NewToolIndex);
}