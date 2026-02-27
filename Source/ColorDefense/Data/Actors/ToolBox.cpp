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
	// PlayerBlock = Cast<APlayerBlock>(PlayerBlockComponent->GetChildActor());
	// Turret = Cast<ATurret>(TurretComponent->GetChildActor());

    // Put the tools in the box.
    ToolBox.Add(CoreRemover);
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
	EnhancedInputComponent->BindAction(ChangeColorAction, ETriggerEvent::Triggered, this, &AToolBox::HandleChangeColor);
	EnhancedInputComponent->BindAction(SwitchToolAction, ETriggerEvent::Triggered, this, &AToolBox::HandleSwitchTool);
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
    int32 ColorIndex = InputValue;
    EColor NewColor = Utils::IndexToColor(ColorIndex);

	CurrentTool->ChangeColor(NewColor);
}

void AToolBox::HandleSwitchTool(const FInputActionValue& Value)
{
    float InputValue = Value.Get<float>();
    int32 ToolIndex = InputValue;

    switch (ToolIndex) 
    {
        case 1 : 
        case 2 : 
        case 3 : 
        case 4 : 
        case 5 : 
        case 6 : 
        case 7 : 
        {
            this->CoreRemover->SwitchToolFrom(CurrentTool); 
            CurrentState = EPlayerState::HoldingCoreRemover;
            break;
        }
        // case 8 : 
        // {
        //     this->PlayerBlock->SwitchToolFrom(CurrentTool); 
        //     CurrentState = EPlayerState::HoldingBlock;
        //     break;   
        // }
        // case 9 :
        // {
        //     this->PlayerBlock->SwitchToolFrom(CurrentTool); 
        //     CurrentState = EPlayerState::HoldingTurret;
        //     break;   
        // }
     default : return;
    }
}