#pragma once

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
}