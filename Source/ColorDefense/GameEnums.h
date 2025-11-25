#pragma once

UENUM()
enum class EColor 
{
    Red     ,
    Orange  ,
    Yellow  ,
    Green   ,
    Blue    ,
    Purple  ,
    Indigo
};

UENUM()
enum class EVoxelProperty : int32
{
    NormalCreepWay,
    CreepStart,
    CreepCheckPoint,
    CreepEnd,
    CreepGenerator,
    Empty
};