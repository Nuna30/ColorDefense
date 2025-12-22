// Fill out your copyright notice in the Description page of Project Settings.

#include "Creep.h"

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

// Called when the game starts or when spawned
void ACreep::BeginPlay()
{
	Super::BeginPlay();

}

void ACreep::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

	// AIController 가져오기
	AIController = Cast<AAIController>(GetController());

	if (AIController && CreepGuide) CreepGuide->GuideCreep(AIController, DontMove, RailNumber);
}

void ACreep::HandleDestruction()
{
    if (VFX)
    {
        // 1. 나이아가라 시스템 스폰 및 컴포넌트 반환 받기
        UNiagaraComponent* SpawnedEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            VFX, // NS_PaintSplash 에셋
            GetActorLocation(),         // Creep의 월드 위치
            GetActorRotation(),         // Creep의 회전
            FVector(1.0f),              // 스케일
            true                        // Auto Destroy (재생 완료 후 자동 파괴)                
        );

        // 2. 컴포넌트가 유효하면 색상 변경 로직 수행
        if (SpawnedEffect)
        {
            FLinearColor TargetColor;

            // GameEnums.h의 EColor에 맞춰 색상 지정
            switch (CreepColor)
            {
            case EColor::Red:
                TargetColor = FLinearColor::Red;
                break;

            case EColor::Orange:
                TargetColor = FLinearColor(1.0f, 0.5f, 0.0f); // 주황색 (R, G, B)
                break;

            case EColor::Yellow:
                TargetColor = FLinearColor::Yellow;
                break;

            case EColor::Green:
                TargetColor = FLinearColor::Green;
                break;

            case EColor::Blue:
                TargetColor = FLinearColor::Blue;
                break;

            case EColor::Purple:
                TargetColor = FLinearColor(0.5f, 0.0f, 1.0f); // 보라색
                break;

            case EColor::Indigo:
                TargetColor = FLinearColor(0.29f, 0.0f, 0.51f); // 남색 (인디고)
                break;

            default:
                TargetColor = FLinearColor::White; // 예외 상황 시 흰색
                break;
            }

            // 3. 나이아가라의 "User.Color" 변수에 색상 주입
            // (나이아가라 에디터에서 만든 변수 이름과 정확히 같아야 함)
            SpawnedEffect->SetVariableLinearColor(FName("User.Linear Color"), TargetColor);
        }
    }

	Destroy();
}

// 헤더에 선언된 ChangeColor 구현
void ACreep::ChangeColor(EColor Color)
{
    CreepColor = Color;

    FLinearColor TargetColor;
    switch (CreepColor)
    {
    case EColor::Red:    TargetColor = FLinearColor::Red; break;
    case EColor::Orange: TargetColor = FLinearColor(1.0f, 0.5f, 0.0f); break;
    case EColor::Yellow: TargetColor = FLinearColor::Yellow; break;
    case EColor::Green:  TargetColor = FLinearColor::Green; break;
    case EColor::Blue:   TargetColor = FLinearColor::Blue; break;
    case EColor::Purple: TargetColor = FLinearColor(0.5f, 0.0f, 1.0f); break;
    case EColor::Indigo: TargetColor = FLinearColor(0.29f, 0.0f, 0.51f); break;
    default:             TargetColor = FLinearColor::White; break;
    }

    if (CreepMesh)
    {
        // 0번 인덱스(첫 번째 머티리얼)에 대해 다이내믹 머티리얼 인스턴스를 생성하거나 가져옵니다.
        UMaterialInstanceDynamic* DynMaterial = CreepMesh->CreateAndSetMaterialInstanceDynamic(0);

        if (DynMaterial)
        {
            // [주의] "Color" 부분은 1단계에서 확인한 머티리얼 파라미터 이름과 정확히 같아야 합니다!
            // 예: 머티리얼 파라미터 이름이 "BaseColor"라면 "BaseColor"로 수정하세요.
            DynMaterial->SetVectorParameterValue(FName("BaseColor"), TargetColor);
        }
    }
}