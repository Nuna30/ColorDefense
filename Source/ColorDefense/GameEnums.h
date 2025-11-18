#ifndef GAMEENUMS_H
#define GAMEENUMS_H

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

enum class EVoxelProperty : int32
{
    NormalCreepWay,
    CreepStart,
    CreepCheckPoint,
    CreepEnd,
    Empty
};

#endif