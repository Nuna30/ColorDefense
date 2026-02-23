// Fill out your copyright notice in the Description page of Project Settings.

#include "CreepWayHandler.h"
#include "GeneratorManagers/ChunkGeneratorManager.h"
#include "GeneratorManagers/CreepWayGeneratorManager.h"

UCreepWayHandler::UCreepWayHandler()
{

}

void UCreepWayHandler::Initialize()
{
	// Get managers.
	UChunkGeneratorManager* ChunkGeneratorManager = GetWorld()->GetSubsystem<UChunkGeneratorManager>();	
	UCreepWayGeneratorManager* CreepWayGeneratorManager = GetWorld()->GetSubsystem<UCreepWayGeneratorManager>();

	// Get generators.
	this->ChunkGenerator = ChunkGeneratorManager->ChunkGenerator;
	this->CreepWayGenerator = CreepWayGeneratorManager->CreepWayGenerator;
}

void UCreepWayHandler::BuildCreepWay()
{
	UE_LOG(LogTemp, Warning, TEXT(" "));
	UE_LOG(LogTemp, Warning, TEXT("Build CreepWay"));
	UE_LOG(LogTemp, Warning, TEXT(" "));

	if (!this->ChunkGenerator->GenerateNextChunk()) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Nowhere to go...")));
		return;
	}

	this->CreepWayGenerator->DirectionContainer = this->ChunkGenerator->DirectionContainer;
	this->CreepWayGenerator->GenerateNextCreepWay();
}

void UCreepWayHandler::DestructCreepWay()
{
	UE_LOG(LogTemp, Warning, TEXT(" "));
	UE_LOG(LogTemp, Warning, TEXT("Destruct CreepWay"));
	UE_LOG(LogTemp, Warning, TEXT(" "));
	
	if (!this->CreepWayGenerator->DeleteCurrentCreepWay()) 
	{
		return;
	}
	
	this->ChunkGenerator->DeleteCurrentChunk();
}