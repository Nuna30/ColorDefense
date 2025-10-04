// Fill out your copyright notice in the Description page of Project Settings.


#include "CreepPoolSubsystem.h"
#include "Creep.h"

void UCreepPoolSubsystem::AddCreep(ACreep* Creep)
{
    if (Creep) Creeps.Add(Creep);
    printCreeps();
}

void UCreepPoolSubsystem::RemoveCreep(ACreep* Creep)
{
    Creeps.Remove(Creep);
}

void UCreepPoolSubsystem::printCreeps()
{
    for (ACreep* Creep : Creeps)
    {
        if (Creep)
        {
            FString CreepName = Creep->GetName();
            UE_LOG(LogTemp, Log, TEXT("Creep Name: %s"), *CreepName);
        }
    }
}