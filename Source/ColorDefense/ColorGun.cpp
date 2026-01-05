// Fill out your copyright notice in the Description page of Project Settings.

#include "ColorGun.h"

// Sets default values for this component's properties
UColorGun::UColorGun()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UColorGun::BeginPlay()
{
	Super::BeginPlay();

	// ColorGun 찾기
	if (GunMeshComponent == nullptr)
    {
        AActor* Owner = GetOwner();
        if (Owner)
        {
            // 방법 C: (추천) Owner가 가진 컴포넌트 중 특정 이름을 가진 녀석 찾기
			// *(BP_PlayerCharacter / ColorGun 씬컴포넌트 / SM_ColorGun과 연결됨)*
            TArray<UStaticMeshComponent*> MeshComps;
            Owner->GetComponents(MeshComps);
            
            for (UStaticMeshComponent* Mesh : MeshComps)
            {
                // 블루프린트에서 지어준 이름이 "SM_Gun" 이라면
                if (Mesh->GetName().Contains("SM_ColorGun") || Mesh->ComponentTags.Contains("PlayerGun"))
                {
                    GunMeshComponent = Mesh;
                    break;
                }
            }
        }
    }
}

// Called every frame
void UColorGun::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UColorGun::Shoot()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return;

	AController* OwnerController = OwnerPawn->GetController();
	if (OwnerController == nullptr) return;

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);

	FVector End = Location + Rotation.Vector() * MaxRange;

	FHitResult Hit;
	// ECC_GameTraceChannel1 등 프로젝트 설정에 맞는 채널 사용
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1);

	if (bSuccess)
	{
		// 팁: 여기서 CurrentColor와 맞은 크립의 색깔을 비교하는 로직을 나중에 추가할 수 있습니다.
		ACreep* HitCreep = Cast<ACreep>(Hit.GetActor());
		if (HitCreep)
		{
			HitCreep->HandleDestruction();
		}
	}
}

// 번호키 누르면 컬러건 색상 변경 구현
void UColorGun::ChangeGunColor(EColor NewColor)
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
	}
}