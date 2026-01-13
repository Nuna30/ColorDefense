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

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
    HoldingColorGun   UMETA(DisplayName = "Holding ColorGun"),
    HoldingBlock  UMETA(DisplayName = "Holding Block")
};
