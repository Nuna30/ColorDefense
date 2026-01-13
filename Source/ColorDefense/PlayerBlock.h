// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
    
#include "Tool.h"
#include "Voxel.h"
#include "DrawDebugHelpers.h"
#include "CoreMinimal.h"
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
    virtual void Tick(float DeltaTime) override;
    virtual void LeftClick() override;
    virtual void RightClick() override;

    UFUNCTION()
	void HandleDestruction();

    UFUNCTION()
    void ShowPreview(EPlayerState PlayerState);

    UFUNCTION()
    void SetOpacity(float Percentage);

public:
    UPROPERTY(VisibleAnywhere, Category = "Mesh")
    USceneComponent* DefaultRoot;

    UPROPERTY(VisibleAnywhere, Category = "Mesh")
    UStaticMeshComponent* BlockMeshComponent;

    UPROPERTY()
    APlayerBlock* CurrentFocusedBlock; // used in ShowPreview

    UPROPERTY(EditAnywhere, Category = "Setting")
    float MaxRange = 5000.0f;

    UPROPERTY()
    bool Placed = false;
    
    FVoxel Voxel;
};