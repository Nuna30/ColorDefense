// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameEnums.h" // EChunkProperty 사용하기 위해
#include "CoreMinimal.h"
#include "Chunk.generated.h"

USTRUCT()
struct FChunk
{
GENERATED_BODY()

public:
    UPROPERTY()
    FIntVector Index;
    UPROPERTY()
    EChunkProperty Property;
public:
    FChunk();
    FChunk(const FIntVector& InChunkIndex, EChunkProperty InProperty);
};