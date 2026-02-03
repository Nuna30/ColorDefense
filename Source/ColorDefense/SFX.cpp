// Fill out your copyright notice in the Description page of Project Settings.


#include "SFX.h"

// Sets default values for this component's properties
USFX::USFX()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USFX::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* Owner = GetOwner()) // 부착된 액터
    {
        Owner->OnDestroyed.AddDynamic(this, &USFX::OnOwnerDestroyed);
    }
	
}


// Called every frame
void USFX::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USFX::OnOwnerDestroyed(AActor* DestroyedActor)
{
    if (ACreep* Creep = Cast<ACreep>(DestroyedActor))
    {
        // CreepColor와 위치 접근 가능
        PlayDestroySFX(Creep->CreepColor, Creep->GetActorLocation());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("DestroyedActor is not a Creep!"));
    }
}

void USFX::PlayDestroySFX(EColor CreepColor, FVector ActorLocation)
{
	UGameplayStatics::PlaySoundAtLocation(
		this,                    // WorldContextObject
		DestroySFX,         	 // Sound
		ActorLocation,           // Location
		1.0f,                    // Volume
		ColorToPitch(CreepColor) // Pitch
	);
}

float USFX::ColorToPitch(EColor CreepColor)
{
	float Pitch = 1.f;

	switch (CreepColor)
	{
		case EColor::Orange : Pitch = FMath::Pow(2.f, 2.f / 12.f); break;
		case EColor::Yellow : Pitch = FMath::Pow(2.f, 4.f / 12.f); break;
		case EColor::Green : Pitch = FMath::Pow(2.f, 5.f / 12.f); break;
		case EColor::Blue : Pitch = FMath::Pow(2.f, 7.f / 12.f); break;
		case EColor::Purple : Pitch = FMath::Pow(2.f, 9.f / 12.f); break;
		case EColor::Indigo : Pitch = FMath::Pow(2.f, 11.f / 12.f); break;
	}

	return Pitch;
}
