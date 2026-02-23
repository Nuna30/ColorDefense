// Fill out your copyright notice in the Description page of Project Settings.

#include "BPActorPool.h"
#define DEBUGMODE 1

void UBPActorPool::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

	for (FString BPActorPath : BPActorPathPool)
	{
		this->Pool.Add(LoadClass<AActor>(nullptr, *BPActorPath));
		if (!this->Pool.Top()) 
		{
			if (DEBUGMODE)
			{
				FString DebugMessage = FString::Printf(TEXT("UBPActorPool::Initialize {%s Initializing Failed.}"), *BPActorPath);
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, DebugMessage);
				UE_LOG(LogTemp, Error, TEXT("%s"), *DebugMessage);
			}
			return;
		}
	}
}