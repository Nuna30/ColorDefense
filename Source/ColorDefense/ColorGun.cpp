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
}

// Called when the game starts
void AColorGun::BeginPlay()
{
	Super::BeginPlay();
}

void AColorGun::LeftClick()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;

	// 플레이어의 사격 범위를 계산해야함
	FVector Location;
	FRotator Rotation;
	AController* OwnerController = OwnerPawn->GetController();
	if (OwnerController == nullptr) return;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	FVector End = Location + Rotation.Vector() * MaxRange;

	FHitResult Hit;
	// ECC_GameTraceChannel1 등 프로젝트 설정에 맞는 채널 사용
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1);

	if (bSuccess)
	{
		ACreep* HitCreep = Cast<ACreep>(Hit.GetActor());
		// 크립이 맞았는지 확인
		if (HitCreep)
		{
			// 색상이 일치할 때만 파괴 로직 실행
			if (HitCreep->CreepColor == CurrentColor)
			{
				HitCreep->HandleDestruction();
			}
            else
            {
                // (선택 사항) 색깔이 다를 때 팅겨내는 소리나 이펙트를 여기에 추가할 수 있습니다.
                // UE_LOG(LogTemp, Warning, TEXT("Wrong Color! Gun: %d, Creep: %d"), (int)CurrentColor, (int)HitCreep->CreepColor);
            }
		}
	}

	DrawDebugLine(GetWorld(), Location, End, FColor::Red, false, 2.0f, 0, 1.0f);
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