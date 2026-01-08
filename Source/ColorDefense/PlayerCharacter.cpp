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

    CurrentState = EPlayerState::HoldingColorGun;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    // RETRIEVE the actors from the components
    // The components automatically spawned these actors before BeginPlay ran
    ColorGun = Cast<AColorGun>(ColorGunComponent->GetChildActor());
    PlayerBlock = Cast<APlayerBlock>(PlayerBlockComponent->GetChildActor());

    // Initialize Tools (Safety Check)
    if (ColorGun) 
    {
        // Ensure the gun knows who owns it (important for Shoot logic)
        ColorGun->SetOwner(this);
        ColorGun->UnEquip(); 
    }
    
    if (PlayerBlock)
    {
        PlayerBlock->SetOwner(this);
        PlayerBlock->UnEquip();
    }

    // 5. Set Initial State
    CurrentState = EPlayerState::HoldingColorGun;
    CurrentTool = ColorGun;

    if (CurrentTool) CurrentTool->Equip();
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
	PlayerInputComponent->BindAction(TEXT("Use"), EInputEvent::IE_Pressed, this, &APlayerCharacter::Use);

	// 컬러건 색깔 변경 바인딩
	PlayerInputComponent->BindAction(TEXT("SwapColorGunRed"), IE_Pressed, this, &APlayerCharacter::SetColorRed);
    PlayerInputComponent->BindAction(TEXT("SwapColorGunOrange"), IE_Pressed, this, &APlayerCharacter::SetColorOrange);
    PlayerInputComponent->BindAction(TEXT("SwapColorGunYellow"), IE_Pressed, this, &APlayerCharacter::SetColorYellow);
    PlayerInputComponent->BindAction(TEXT("SwapColorGunGreen"), IE_Pressed, this, &APlayerCharacter::SetColorGreen);
    PlayerInputComponent->BindAction(TEXT("SwapColorGunBlue"), IE_Pressed, this, &APlayerCharacter::SetColorBlue);
    PlayerInputComponent->BindAction(TEXT("SwapColorGunIndigo"), IE_Pressed, this, &APlayerCharacter::SetColorIndigo);
    PlayerInputComponent->BindAction(TEXT("SwapColorGunPurple"), IE_Pressed, this, &APlayerCharacter::SetColorPurple);

	// Bind the T key to switch tools
    PlayerInputComponent->BindAction(TEXT("SwitchToBlocks"), IE_Pressed, this, &APlayerCharacter::SwitchToBlocks);
}

void APlayerCharacter::MoveForward(float AxisValue)
{
	// 무브먼트 컴포넌트(Movement Component)에 이동 요청을 추가
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void APlayerCharacter::LookUp(float AxisValue)
{
	// 컨트롤러에 pitch 값 추가
	AddControllerPitchInput(AxisValue);
}

void APlayerCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

void APlayerCharacter::LookRight(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

void APlayerCharacter::Jump()
{
	ACharacter::Jump();
}

void APlayerCharacter::RequestChangeColor(EColor NewColor)
{
	if (CurrentState != EPlayerState::HoldingColorGun) return;

	ColorGun->ChangeGunColor(NewColor);
}

void APlayerCharacter::SetColorRed()    { RequestChangeColor(EColor::Red); }
void APlayerCharacter::SetColorOrange() { RequestChangeColor(EColor::Orange); }
void APlayerCharacter::SetColorYellow() { RequestChangeColor(EColor::Yellow); }
void APlayerCharacter::SetColorGreen()  { RequestChangeColor(EColor::Green); }
void APlayerCharacter::SetColorBlue()   { RequestChangeColor(EColor::Blue); }
void APlayerCharacter::SetColorIndigo() { RequestChangeColor(EColor::Indigo); }
void APlayerCharacter::SetColorPurple() { RequestChangeColor(EColor::Purple); }

// Inside Use()
void APlayerCharacter::Use()
{
    if (CurrentTool) 
    {
        CurrentTool->Use(); // If it's the gun, it shoots. If it's the block, it places.
    }

    if (CurrentState == EPlayerState::HoldingColorGun)
    {
        OnShoot();
    }
}

void APlayerCharacter::SwitchToBlocks()
{
    SwitchTool(); 
}

void APlayerCharacter::SwitchTool()
{
    if (CurrentTool) CurrentTool->UnEquip();

    if (CurrentState == EPlayerState::HoldingColorGun)
    {
        CurrentState = EPlayerState::HoldingBlock;
        CurrentTool = PlayerBlock;
    }
    else
    {
        CurrentState = EPlayerState::HoldingColorGun;
        CurrentTool = ColorGun;
    }

    if (CurrentTool) CurrentTool->Equip();
}