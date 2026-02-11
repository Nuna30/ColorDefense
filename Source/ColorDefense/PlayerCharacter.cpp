// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "CreepWayHandler.h"
#include "Components/CanvasPanel.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/ShopWidget.h"

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

    // Reset Controller
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        // 1. Switch back to Game Only
        FInputModeGameOnly InputModeData;
        PC->SetInputMode(InputModeData);

        // 2. Hide the mouse
        PC->bShowMouseCursor = false;
    }

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

    // Update 20 times per second (0.05s interval) instead of every frame
	// Better than Tick
    FTimerHandle TimerHandle;
    FTimerDelegate TimerDelegate;
    TimerDelegate.BindLambda([this](){PlayerBlock->ShowPreview(this->CurrentState);});
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.05f, true);

    // Dynamic Creep Path Generation Test
    this->CreepWayHandler = NewObject<UCreepWayHandler>(this, UCreepWayHandler::StaticClass());
	this->CreepWayHandler->Initialize();

    // Shopping
    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (ShopWidgetClass)
        {
            ShopWidgetInstance = CreateWidget<UShopWidget>(PC, ShopWidgetClass);
            if (ShopWidgetInstance)
            {
                ShopWidgetInstance->AddToViewport(10);
                ShopWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
            }
        }
    }
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
    PlayerInputComponent->BindAction(TEXT("LeftClick"), EInputEvent::IE_Released, this, &APlayerCharacter::HandleLeftClickReleased);
    PlayerInputComponent->BindAction(TEXT("RightClick"), EInputEvent::IE_Pressed, this, &APlayerCharacter::HandleRightClick);

    // EnhancedInputComponent
    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // ColorGun
        EnhancedInputComponent->BindAction(ChangeColorAction, ETriggerEvent::Triggered, this, &APlayerCharacter::HandleChangeColor);
        EnhancedInputComponent->BindAction(SwitchToolAction, ETriggerEvent::Triggered, this, &APlayerCharacter::HandleSwitchTool);
        // Shopping
        EnhancedInputComponent->BindAction(ToggleShopAction, ETriggerEvent::Started, this, &APlayerCharacter::ToggleShop);
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

void APlayerCharacter::HandleLeftClickReleased()
{
    if (CurrentTool)
    {
        CurrentTool->LeftClickReleased();
        this->CreepWayHandler->BuildCreepWay();
    }
}

void APlayerCharacter::HandleRightClick()
{
    CurrentTool->RightClick(); 
    this->CreepWayHandler->DestructCreepWay();
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
        case 7 : 
        {
            this->ColorGun->SwitchToolFrom(CurrentTool); 
            CurrentState = EPlayerState::HoldingColorGun;
            break;
        }
        case 8 : 
        {
            this->PlayerBlock->SwitchToolFrom(CurrentTool); 
            CurrentState = EPlayerState::HoldingBlock;
            break;   
        }
        case 9 :
        {
            this->PlayerBlock->SwitchToolFrom(CurrentTool); 
            CurrentState = EPlayerState::HoldingTurret;
            break;   
        }
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

// --- Shopping --- //

void APlayerCharacter::ToggleShop()
{
    if (!ShopWidgetInstance) return;

    APlayerController* PC = Cast<APlayerController>(GetController());
    if (!PC) return;

    if (ShopWidgetInstance->GetVisibility() == ESlateVisibility::Visible)
    {
        UE_LOG(LogTemp, Warning, TEXT("Close the Shop"));
        ShopWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
        PC->bShowMouseCursor = false;
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Open the Shop"));
        
        // Force the root widget to visible
        ShopWidgetInstance->SetVisibility(ESlateVisibility::Visible);
        
        // Force the CanvasPanel to visible just in case
        if (ShopWidgetInstance->ShopCanvasPanel)
        {
            ShopWidgetInstance->ShopCanvasPanel->SetVisibility(ESlateVisibility::Visible);
        }

        PC->bShowMouseCursor = true;
        FInputModeGameAndUI InputMode;
        InputMode.SetWidgetToFocus(ShopWidgetInstance->TakeWidget());
        PC->SetInputMode(InputMode);
    }
}