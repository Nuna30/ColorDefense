// Fill out your copyright notice in the Description page of Project Settings.


#include "Creep.h"
#include "AIController.h"
#include "CreepPoolSubsystem.h"
#include "CreepGuide.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACreep::ACreep()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CreepGuide = CreateDefaultSubobject<UCreepGuide>(TEXT("Creep Guide"));
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

	if (AIController) CreepGuide->GuideCreep(AIController, DontMove);
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

	if (SFX)
    {
        // 월드 특정 위치에서 사운드 재생 (일반적으로 Actor의 위치)
        UGameplayStatics::PlaySoundAtLocation(
            this,                 // 월드 컨텍스트
            SFX,     // 연결된 사운드 에셋
            GetActorLocation()    // 사운드가 발생할 위치
        );
    }

	CreepPool->RemoveCreep(this);

	Destroy();
}