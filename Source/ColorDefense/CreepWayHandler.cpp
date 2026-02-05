// Fill out your copyright notice in the Description page of Project Settings.

#include "CreepWayHandler.h"
#include "ChunkGeneratorManager.h"
#include "CreepWayGeneratorManager.h"

UCreepWayHandler::UCreepWayHandler()
{

}

void UCreepWayHandler::Initialize()
{
	// Get managers.
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	UChunkGeneratorManager* ChunkGeneratorManager = GameInstance->GetSubsystem<UChunkGeneratorManager>();	
	UCreepWayGeneratorManager* CreepWayGeneratorManager = GameInstance->GetSubsystem<UCreepWayGeneratorManager>();

	// Get generators.
	this->ChunkGenerator = ChunkGeneratorManager->ChunkGenerator;
	this->CreepWayGenerator = CreepWayGeneratorManager->CreepWayGenerator;
}

void UCreepWayHandler::BuildCreepWay()
{
	bool bBlocked = false;
	this->ChunkGenerator->GenerateNextChunk(bBlocked);

	if (bBlocked) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Nowhere to go...")));
		return;
	}

	this->CreepWayGenerator->DirectionContainer = this->ChunkGenerator->DirectionContainer;
	this->CreepWayGenerator->GenerateNextCreepWay();
}

void UCreepWayHandler::DestructCreepWay()
{
	bool bNoMoreDestruction = false;
	CreepWayGenerator->DeleteCurrentCreepWay(bNoMoreDestruction);

	if (bNoMoreDestruction) return;

	this->ChunkGenerator->DeleteCurrentChunk();
}