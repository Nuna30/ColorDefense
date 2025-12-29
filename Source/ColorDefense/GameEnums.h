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
enum class EVoxelProperty
{
    NormalCreepWay,
    CreepStart,
    CreepCheckPoint,
    CreepEnd,
    CreepGenerator,
    Empty
};

UENUM()
enum class EChunkProperty
{
    CreepWay,
    Empty
};