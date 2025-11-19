// Fill out your copyright notice in the Description page of Project Settings.

#include "Voxel.h"

FVoxel::FVoxel()
{
	this->Transform = FTransform::Identity;
	this->BPActor = nullptr;
	this->Property = EVoxelProperty::Empty;
	this->Index = FIntVector::ZeroValue;
    this->SpawnedActor = nullptr;
}