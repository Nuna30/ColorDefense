// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create Camera First
    FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
    FPSCamera->SetupAttachment(RootComponent);
    FPSCamera->bUsePawnControlRotation = true; // IMPORTANT: Follow mouse look

    // Create the Arm
    InvisibleArm = CreateDefaultSubobject<USceneComponent>(TEXT("InvisibleArm"));
    InvisibleArm->SetupAttachment(FPSCamera);

    // Create the Child Actor Components
    // These will hold the actual Gun and Block actors
    ColorGunComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("ColorGunComponent"));
    ColorGunComponent->SetupAttachment(InvisibleArm);

    PlayerBlockComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("PlayerBlockComponent"));
    PlayerBlockComponent->SetupAttachment(InvisibleArm);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    // RETRIEVE the actors from the components
    // The components automatically spawned these actors before BeginPlay ran
    ColorGun = Cast<AColorGun>(ColorGunComponent->GetChildActor());
    PlayerBlock = Cast<APlayerBlock>(PlayerBlockComponent->GetChildActor());

    // All set hidden except color gun
    Tools.Add(ColorGun);
    Tools.Add(PlayerBlock);

    // Initialize Tools
    for (ATool* Tool : Tools)
    {
        Tool->SetOwner(this);
        Tool->UnEquip();
    }

    // Add Input Mapping Context
    if (APlayerController* PC = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }

    // Set Initial State
    CurrentState = EPlayerState::HoldingColorGun;
    CurrentTool = ColorGun;
    CurrentTool->SwitchToolFrom(CurrentTool);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 키 바인딩 매핑
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APlayerCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APlayerCharacter::LookRight);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("LeftClick"), EInputEvent::IE_Pressed, this, &APlayerCharacter::HandleLeftClick);
    PlayerInputComponent->BindAction(TEXT("RightClick"), EInputEvent::IE_Pressed, this, &APlayerCharacter::HandleRightClick);

	// Color Gun Input System
    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Bind the single action
        EnhancedInputComponent->BindAction(ChangeColorAction, ETriggerEvent::Triggered, this, &APlayerCharacter::HandleChangeColor);
        EnhancedInputComponent->BindAction(SwitchToolAction, ETriggerEvent::Triggered, this, &APlayerCharacter::HandleSwitchTool);
    }
}

// Inside LeftClick()
void APlayerCharacter::HandleLeftClick()
{
    CurrentTool->LeftClick(); // If it's the gun, it shoots. If it's the block, it places.

    // I had to make another colorgun branch here because the shooting animation is handled in the PlayerCharacter blueprint.
    // I don't think this is the best way, so I should find a better approach.
    if (CurrentState == EPlayerState::HoldingColorGun) OnShoot();
}

void APlayerCharacter::HandleRightClick()
{
    CurrentTool->RightClick(); 
}

void APlayerCharacter::HandleChangeColor(const FInputActionValue& Value)
{
    // We get a float from the input (1.0 for Key 1, 2.0 for Key 2, etc.)
    float InputValue = Value.Get<float>();
    int32 ColorIndex = InputValue;
    EColor NewColor = EColor::Red;

    // Convert float to our Enum
    switch (ColorIndex) 
    {
        case 1 : NewColor = EColor::Red; break;
        case 2 : NewColor = EColor::Orange; break;
        case 3 : NewColor = EColor::Yellow; break;
        case 4 : NewColor = EColor::Green; break;
        case 5 : NewColor = EColor::Blue; break;
        case 6 : NewColor = EColor::Indigo; break;
        case 7 : NewColor = EColor::Purple; break;
        default : return;
    }

    if (CurrentState != EPlayerState::HoldingColorGun)
    {
        ColorGun->SwitchToolFrom(CurrentTool);
        CurrentState = EPlayerState::HoldingColorGun;
    }

	ColorGun->ChangeGunColor(NewColor);
}

void APlayerCharacter::HandleSwitchTool(const FInputActionValue& Value)
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
        case 7 : this->ColorGun->SwitchToolFrom(CurrentTool); break;
        case 8 : this->PlayerBlock->SwitchToolFrom(CurrentTool); break;
        // case 9 : NextTool = this->Turret; break;
     default : return;
    }

}

void APlayerCharacter::MoveForward(float AxisValue)
{
	// 무브먼트 컴포넌트(Movement Component)에 이동 요청을 추가
	AddMovementInput(GetActorForwardVector(), AxisValue);
}

void APlayerCharacter::LookUp(float AxisValue)
{
	// 컨트롤러에 pitch 값 추가
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