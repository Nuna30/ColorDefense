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
	this->ChunkGenerator->GenerateNextChunk();
	this->CreepWayGenerator->DirectionContainer = this->ChunkGenerator->DirectionContainer;
	this->CreepWayGenerator->GenerateNextCreepWay();
}