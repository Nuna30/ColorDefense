// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tool.h"
#include "PlayerBlock.generated.h"

/**
 * 
 */
UCLASS()
class COLORDEFENSE_API APlayerBlock : public ATool
{
	GENERATED_BODY()
public:
	APlayerBlock();
	virtual void BeginPlay() override;

    virtual void Use() override;
	
public:
    UPROPERTY(VisibleAnywhere, Category = "Mesh")
    UStaticMeshComponent* BlockMeshComponent;

};
