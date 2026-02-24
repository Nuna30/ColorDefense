#pragma once

#include "GameEnums.h"
#include "CoreMinimal.h"

namespace Utils
{
    // 'inline' allows this to be defined in the header without "multiple definition" errors
    inline const TArray<FIntVector> CardinalDirections = {
        FIntVector(1, 0, 0),  // Right
        FIntVector(-1, 0, 0), // Left
        FIntVector(0, 1, 0),  // Up
        FIntVector(0, -1, 0)  // Down
    };

    inline FIntVector GetPerpendicularDirection(const FIntVector& Direction)
    {
        if (Direction.X == 1) return FIntVector(0, 1, Direction.Z);
        else if (Direction.Y == 1) return FIntVector(1, 0, Direction.X);
        else return FIntVector::ZeroValue;
    }

    inline FLinearColor GetLinearColor(EColor Color)
    {
        FLinearColor TargetColor;
        switch (Color)
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
        return TargetColor;
    }

    inline float ColorToPitch(EColor CreepColor)
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

    inline bool GetHit(AActor* CallingActor, float InMaxRange, FHitResult& Hit, ECollisionChannel CollisionChannel)
    {
        if (!CallingActor) return false;  // Null check

        // Get Player's ViewPoint from the calling actor's owner.
        AController* Controller = CallingActor->GetOwner() ? Cast<APawn>(CallingActor->GetOwner())->GetController() : nullptr;
        if (!Controller) return false;

        FVector Location;
        FRotator Rotation;
        Controller->GetPlayerViewPoint(Location, Rotation);
        FVector End = Location + Rotation.Vector() * InMaxRange;

        // Using the calling actor's world
        if (CallingActor->GetWorld()->LineTraceSingleByChannel(Hit, Location, End, CollisionChannel)) return true;
        else return false;
    }
}