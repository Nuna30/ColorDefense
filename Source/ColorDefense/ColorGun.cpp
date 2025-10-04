// Fill out your copyright notice in the Description page of Project Settings.


#include "ColorGun.h"
#include "Creep.h"

// Sets default values for this component's properties
UColorGun::UColorGun()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UColorGun::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UColorGun::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
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
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Location, End, ECollisionChannel::ECC_GameTraceChannel1);

	if (bSuccess)
	{
		// DrawDebugPoint(GetWorld(), Hit.Location, 20, FColor::Red, true);
		Cast<ACreep>(Hit.GetActor())->HandleDestruction();
		// GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, FString::Printf(TEXT("%s"), *(Hit.GetActor()->GetName())));
	}

}