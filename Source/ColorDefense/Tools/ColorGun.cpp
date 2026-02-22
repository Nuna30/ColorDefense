// Fill out your copyright notice in the Description page of Project Settings.

#include "ColorGun.h"

// Sets default values for this component's properties
AColorGun::AColorGun()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create Components for the Actor
    DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
    RootComponent = DefaultRoot;

    GunMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
    GunMeshComponent->SetupAttachment(RootComponent);

	ComboAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("ComboAudioComponent"));
    ComboAudioComponent->SetupAttachment(RootComponent);
    ComboAudioComponent->bAutoActivate = false; // Don't play immediately
	
}

// Called when the game starts
void AColorGun::BeginPlay()
{
	Super::BeginPlay();
}

void AColorGun::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsConnecting)
    {
        UpdateChain();
    }
}

void AColorGun::LeftClick()
{
	bIsConnecting = true;
    ConnectedCreeps.Empty(); // Clear any previous chain
    UpdateChain(); // Try to catch the first creep immediately
}

void AColorGun::UpdateChain()
{
    APawn* OwnerPawn = Cast<APawn>(GetOwner());
    if (!OwnerPawn) return;

    FVector Location;
    FRotator Rotation;
    OwnerPawn->GetController()->GetPlayerViewPoint(Location, Rotation);
    FVector End = Location + Rotation.Vector() * MaxRange;

    FHitResult Hit;
    // Using your existing trace channel
    if (GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1))
    {
        ACreep* HitCreep = Cast<ACreep>(Hit.GetActor());
    
        // Check if: 1. It's a Creep, 2. Color matches, 3. Not already in our list
		if (HitCreep && HitCreep->CreepColor == CurrentColor && !ConnectedCreeps.Contains(HitCreep))
		{
			// Creep Highlight
            ConnectedCreeps.Add(HitCreep);
            HitCreep->SetHighlighted(true);

			// Update Combo
			ComboCount++;
			PlayComboSound();
			if (AColorDefenseGameState* GS = GetWorld()->GetGameState<AColorDefenseGameState>()) GS->AddCombo(1);
			
		}
    }
}

void AColorGun::LeftClickReleased()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("LeftClickReleased"));
    bIsConnecting = false;

	// Money is honey
	AColorDefenseGameState* GS = GetWorld()->GetGameState<AColorDefenseGameState>();

    // Kill everyone in the chain!
    for (ACreep* Creep : ConnectedCreeps)
    {
        if (IsValid(Creep))
        {
            Creep->HandleDestruction(); // Triggers VFX and Destroy()

			// Add money
			GS->AddMoney(1); 
        }
    }

	// Reset Creep Highlight
    ConnectedCreeps.Empty();

	// Reset ComboCount
	ComboCount = 0;
}

// 번호키 누르면 컬러건 색상 변경 구현
void AColorGun::ChangeGunColor(EColor NewColor)
{
	CurrentColor = NewColor;

	// 1. EColor를 FLinearColor로 변환 (Creep.cpp와 동일한 로직)
	FLinearColor TargetColor;
	switch (CurrentColor)
	{
		case EColor::Red:    TargetColor = FLinearColor::Red; break;
		case EColor::Orange: TargetColor = FLinearColor(1.0f, 0.5f, 0.0f); break;
		case EColor::Yellow: TargetColor = FLinearColor::Yellow; break;
		case EColor::Green:  TargetColor = FLinearColor::Green; break;
		case EColor::Blue:   TargetColor = FLinearColor::Blue; break;
		case EColor::Purple: TargetColor = FLinearColor(0.5f, 0.0f, 1.0f); break; // 보라
		case EColor::Indigo: TargetColor = FLinearColor(0.29f, 0.0f, 0.51f); break; // 남색
		default:             TargetColor = FLinearColor::White; break;
	}

	// 2. GunMeshComponent가 유효한지 확인
	if (GunMeshComponent)
	{
		// 바꾸고 싶은 파츠가 Element Index 4번임. 머티리얼을 동적으로 생성/가져옴.
		UMaterialInstanceDynamic* DynMaterial = GunMeshComponent->CreateAndSetMaterialInstanceDynamic(4);

		if (DynMaterial)
		{
			// Creep과 동일하게 "BaseColor" 파라미터를 사용.
			DynMaterial->SetVectorParameterValue(FName("BaseColor"), TargetColor);
		}

		// 3. 컬러건의 색깔을 바꿈과 동시에 VFX 스폰 (GunMeshComponent가 있어야하므로 nested)
		if (SwapVFX)
		{
			// 1. 나이아가라 시스템 스폰 및 컴포넌트 반환 받기
			UNiagaraComponent* SpawnedEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				SwapVFX, // NS_PaintSplash 에셋
				GunMeshComponent->GetComponentLocation(),
				GunMeshComponent->GetComponentRotation(),
				FVector(1.0f),              // 스케일
				true                        // Auto Destroy (재생 완료 후 자동 파괴)
			);

			// 2. 컴포넌트가 유효하면 색상 변경 로직 수행
			if (SpawnedEffect)
			{
				// 3. 나이아가라의 "User.Color" 변수에 색상 주입
				// (나이아가라 에디터에서 만든 변수 이름과 정확히 같아야 함)
				// SpawnedEffect->SetVariableLinearColor(FName("User.ColorGunSwapColor"), TargetColor);
				SpawnedEffect->SetVariableLinearColor(FName("User.Linear Color"), TargetColor);
			}
		}
	}
}

// --- Combo --- //

void AColorGun::PlayComboSound()
{
	if (ComboSound && ComboAudioComponent)
	{
		// If it's the first hit, set the sound and play
		// if (!ComboAudioComponent->IsPlaying())
		// {
		// 	ComboAudioComponent->SetSound(ComboSound);
		// 	ComboAudioComponent->Play();
		// }
		ComboAudioComponent->SetSound(ComboSound);
		ComboAudioComponent->Play();

		// Increase pitch based on ComboCount
		// Base pitch (1.0) + (ComboCount * Multiplier)
		float NewPitch = 1.0f + (ComboCount * PitchMultiplier);
		ComboAudioComponent->SetPitchMultiplier(NewPitch);
	}
}