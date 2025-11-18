// Fill out your copyright notice in the Description page of Project Settings.


#include "Creep.h"
#include "AIController.h"
#include "CreepPoolSubsystem.h"
#include "CreepGuide.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInterface.h"
#include "SFX.h"

// Sets default values
ACreep::ACreep()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // Mesh 초기화 및 루트 컴포넌트로 설정
    CreepMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CreepMesh"));
    RootComponent = CreepMesh; // 루트로 지정

    // Creep Guide
	CreepGuide = CreateDefaultSubobject<UCreepGuide>(TEXT("CreepGuide"));
}

// Called every frame
void ACreep::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACreep::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// Called when the game starts or when spawned
void ACreep::BeginPlay()
{
	Super::BeginPlay();

	// 크립 풀에 객체 넣기
	CreepPool = GetGameInstance()->GetSubsystem<UCreepPoolSubsystem>();
	CreepPool->AddCreep(this);
}

void ACreep::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

	// AIController 가져오기
	AIController = Cast<AAIController>(GetController());

	if (AIController && CreepGuide) CreepGuide->GuideCreep(AIController, DontMove);
}

void ACreep::HandleDestruction()
{
    if (VFX)
    {
        // 2-1. 특정 위치에 시스템 생성 및 발동 (가장 일반적인 방법)
        // 캐릭터의 위치를 가져와서 발동
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            VFX, // NS_PaintSplash 에셋
            GetActorLocation(),         // 캐릭터의 월드 위치
            GetActorRotation(),         // 캐릭터의 회전
            FVector(1.0f),              // 스케일
            true                        // Auto Destroy (재생 완료 후 자동 파괴)
        );
    }

	CreepPool->RemoveCreep(this);

	Destroy();
}

void ACreep::ChangeColor(EColor Color)
{
    if (!CreepMesh) return; // Mesh가 없으면 바로 리턴

    UMaterialInterface* NewMaterial = nullptr;
    CreepColor = Color;

    // FString을 사용하여 에셋 경로의 접두사를 정의합니다. (TEXT 매크로 사용)
    const FString BasePath = TEXT("/Game/MyAssets/Creep/Materials/");
    FString AssetName = TEXT(""); // FString으로 에셋 이름을 저장할 변수

    switch (Color) 
    {
        case EColor::Red:
            AssetName = TEXT("MI_M_Red.MI_M_Red");
            break;
        case EColor::Orange:
            AssetName = TEXT("MI_M_Orange.MI_M_Orange");
            break;
        case EColor::Yellow:
            AssetName = TEXT("MI_M_Yellow.MI_M_Yellow");
            break;
        case EColor::Green:
            AssetName = TEXT("MI_M_Green.MI_M_Green");
            break;
        case EColor::Blue:
            AssetName = TEXT("MI_M_Blue.MI_M_Blue");
            break;
        case EColor::Purple:
            AssetName = TEXT("MI_M_Purple.MI_M_Purple");
            break;
        case EColor::Indigo:
            AssetName = TEXT("MI_M_Indigo.MI_M_Indigo");
            break;
        default:
            return; // 찾고자 하는 색상이 아니면 함수 종료
    }
    
    // BasePath와 AssetName을 FString의 * 연산자를 이용해 결합합니다.
    const FString FullPath = BasePath + AssetName;

    // LoadObject 함수에 FString의 * 연산자를 통해 TCHAR* 포인터를 넘겨줍니다.
    NewMaterial = LoadObject<UMaterialInterface>(nullptr, *FullPath);

    if (NewMaterial)
    {
        CreepMesh->SetMaterial(0, NewMaterial);
    }
}