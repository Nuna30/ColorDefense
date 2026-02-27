// Fill out your copyright notice in the Description page of Project Settings.
#include "Tool.h"

// ========================= //
// ===== Base Function ===== //
// ========================= //

ATool::ATool()
{
	PrimaryActorTick.bCanEverTick = true;

	// Base Static Mesh Component
    ToolMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tool Mesh Component"));
    ToolMeshComponent->SetupAttachment(RootComponent);
}

void ATool::BeginPlay()
{
	Super::BeginPlay();
}

void ATool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// =============================== //
// ===== Handle Mouse Input ====== //
// =============================== //

void ATool::LeftClick() 
{
	// Left Clicked.
}

void ATool::LeftClickReleased() 
{
	// Left Click Released.
}

void ATool::RightClick() 
{
	// Right Clicked.
}

// ================= //
// ===== Utils ===== //
// ================= //

void ATool::SwitchToolFrom(ATool*& CurrentTool) 
{
	CurrentTool->SetActorHiddenInGame(true);
	SetActorHiddenInGame(false);
	CurrentTool = this;
}

void ATool::UnEquip()
{
	SetActorHiddenInGame(true);
}

// ===================== //
// ====== Features ===== //
// ===================== //

void ATool::ChangeColor(EColor NewColor)
{
	// All tools Change its Color.
	CurrentColor = NewColor;

	// All tools play its SwapVFX.
	UNiagaraComponent* SpawnedEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		SwapVFX,
		ToolMeshComponent->GetComponentLocation(),
		ToolMeshComponent->GetComponentRotation(),
		FVector(1.0f),            
		true                      
	);

	// All tools change its SwapVFX Color.
	SpawnedEffect->SetVariableLinearColor(FName("User.Linear Color"), Utils::GetLinearColor(NewColor));
}