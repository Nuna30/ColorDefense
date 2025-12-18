// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameEnums.h"
#include "Voxel.generated.h"

USTRUCT()
struct FVoxel
{
GENERATED_BODY()

public:
    FVoxel();
public:
    UPROPERTY()
    FIntVector Index;
    UPROPERTY()
    FTransform Transform; // *주의: 이거 하나당 메모리를 꽤 먹습니다(48바이트). 응 어쩔티비 
    UPROPERTY()
    EVoxelProperty Property;
    UPROPERTY()
    TSubclassOf<AActor> BPActor;
    UPROPERTY()
    TWeakObjectPtr<AActor> SpawnedActor;
};