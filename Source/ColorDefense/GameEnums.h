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
    SlopeCreepWay,
    CreepStart,
    CreepCheckPoint,
    CreepEnd,
    CreepGenerator,
    Placeable,
    PlayerBlock,
    Empty
};

UENUM()
enum class EChunkProperty
{
    CreepWay,
    Empty
};

UENUM()
enum class EPlayerState
{
    HoldingColorGun,
    HoldingBlock,
    HoldingTurret
};
