#include "Data/Actors/ToolBox.h"
#include "Utils/Utils.h"
#include "PlayerCharacter.h"

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

    // Put the tools in the box.
    ToolBox.Add(CoreRemover);
    ToolBox.Add(Rifle);
	// ToolBox.Add(PlayerBlock);
	// ToolBox.Add(Turret);

    // Initialize Tools
    for (ATool* Tool : ToolBox)
    {
        // Tool->SetOwner(this);
        Tool->UnEquip();
    }

	// Set up the States
    CurrentState = EPlayerState::HoldingCoreRemover;
    CurrentTool = CoreRemover;
    CurrentTool->SwitchToolFrom(CurrentTool);

}

void AToolBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AToolBox::BindActions(UInputComponent* PlayerInputComponent, APlayerCharacter* OwnerCharacter)
{
	PlayerInputComponent->BindAction(TEXT("LeftClick"),   EInputEvent::IE_Pressed,  this, &AToolBox::HandleLeftClick);
	PlayerInputComponent->BindAction(TEXT("LeftClick"),   EInputEvent::IE_Released, this, &AToolBox::HandleLeftClickReleased);
	PlayerInputComponent->BindAction(TEXT("RightClick"),  EInputEvent::IE_Pressed,  this, &AToolBox::HandleRightClick);
}

void AToolBox::BindCoreRemoverActionsEnhanced(APlayerCharacter* OwnerCharacter)
{
	// Get the Enhanced Input Component from the owning PlayerCharacter
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(OwnerCharacter->InputComponent);

	// Core Remover Enhanced Input
	EnhancedInputComponent->BindAction(ChangeColorAction, ETriggerEvent::Started, this, &AToolBox::HandleChangeColor);
	EnhancedInputComponent->BindAction(SwitchToolAction, ETriggerEvent::Started, this, &AToolBox::HandleSwitchTool);
}

void AToolBox::HandleLeftClick()
{
    CurrentTool->LeftClick();
}

void AToolBox::HandleLeftClickReleased()
{
	CurrentTool->LeftClickReleased();
}

void AToolBox::HandleRightClick()
{
    CurrentTool->RightClick(); 
}

void AToolBox::HandleChangeColor(const FInputActionValue& Value)
{
    // We get a float from the input (1.0 for Key 1, 2.0 for Key 2, etc.)
    float InputValue = Value.Get<float>();
    UE_LOG(LogTemp, Warning, TEXT("InputValue : %.5f"), InputValue);
    int32 ColorIndex = InputValue;
    EColor NewColor = Utils::IndexToColor(ColorIndex);

	CurrentTool->ChangeColor(NewColor);
}

void AToolBox::HandleSwitchTool(const FInputActionValue& Value)
{
    float InputValue = Value.Get<float>();
    int32 ToolIndex = FMath::FloorToInt(InputValue);

    ATool* NewTool = nullptr;
    EPlayerState NewState = CurrentState;

    switch (ToolIndex)
    {
        case 1:
            NewTool = CoreRemover;
            NewState = EPlayerState::HoldingCoreRemover;
            break;
        case 2:
            NewTool = Rifle;
            NewState = EPlayerState::HoldingRifle;
            break;
        case 3:
            // NewTool = PlayerBlock;
            // NewState = EPlayerState::HoldingBlock;
            break;
        case 4:
            // NewTool = Turret;
            // NewState = EPlayerState::HoldingTurret;
            break;

        default:
            return; // Invalid index
    }

    // Only switch if it's a different tool
    if (NewTool && NewTool != CurrentTool)
    {
        NewTool->SwitchToolFrom(CurrentTool);
        CurrentTool = NewTool;
        CurrentState = NewState;

        // Optional: play a nice "tool swapped" sound or VFX here later
    }
}