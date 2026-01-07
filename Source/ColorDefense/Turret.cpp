#include "Turret.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

ATurret::ATurret()
{
    PrimaryActorTick.bCanEverTick = true;

    // Initialize the single TurretMesh as the Root
    TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
    RootComponent = TurretMesh;
}

void ATurret::BeginPlay() { Super::BeginPlay(); }

void ATurret::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FindTarget();

    if (CurrentTarget)
    {
        RotateAndAttack(DeltaTime);
    }
}

void ATurret::FindTarget()
{
    // Check if current target is still valid and in range
    if (CurrentTarget)
    {
        if (FVector::Dist(GetActorLocation(), CurrentTarget->GetActorLocation()) <= Range) 
        {
            return; 
        }
        CurrentTarget = nullptr;
    }

    // Search for the closest ACreep within Range
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));

    TArray<AActor*> IgnoredActors;
    IgnoredActors.Add(this);

    TArray<AActor*> OutActors;
    UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), Range, ObjectTypes, ACreep::StaticClass(), IgnoredActors, OutActors);

    float ClosestDistance = MAX_FLT;
    for (AActor* Actor : OutActors)
    {
        UE_LOG(LogTemp, Warning, TEXT("Actor"));
        if (ACreep* Creep = Cast<ACreep>(Actor))
        {
            float Dist = FVector::Dist(GetActorLocation(), Creep->GetActorLocation());
            if (Dist < ClosestDistance)
            {
                ClosestDistance = Dist;
                CurrentTarget = Creep;
                UE_LOG(LogTemp, Warning, TEXT("Find Target!"));
            }
        }
    }
}

void ATurret::RotateAndAttack(float DeltaTime)
{
    // 1. Calculate Rotation
    FVector Direction = CurrentTarget->GetActorLocation() - GetActorLocation();
    FRotator LookAtRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
    
    // We only want to rotate the Yaw (Left/Right) for the whole body
    FRotator TargetRotation = FRotator(0.f, LookAtRotation.Yaw, 0.f);
    
    // Interpolate for smooth rotation
    FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 5.f);
    SetActorRotation(NewRotation);

    // 2. Draw Laser Beam (Debug)
    DrawDebugLine(GetWorld(), GetActorLocation(), CurrentTarget->GetActorLocation(), FColor::Red, false, -1, 0, 3.f);

    // 3. Kill the Creep!
    CurrentTarget->HandleDestruction();

    // debug
    UE_LOG(LogTemp, Warning, TEXT("RotateAndAttack!"));
}