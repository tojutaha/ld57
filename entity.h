#pragma once
#include "root.h"

#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 64

typedef enum
{
    PlateColor_Red,
    PlateColor_Green,
    PlateColor_Blue,
    PlateColor_Yellow,
    PlateColor_Purple,
    PlateColor_White,
    PlateColor_Orange,
    PlateColor_Cyan,
} PlateColor;

typedef enum
{
    CollisionFlag_None    = 0,
    CollisionFlag_Overlap = 1 << 0,
    CollisionFlag_Block   = 1 << 1,
} CollisionFlag;

typedef enum
{
    None,
    Player,
    Door,
    Wall,
    PressurePlate,
    Clone,

    BeamEmitter,
    Mirror,
} EntityType;

typedef enum
{
    Left = 0,
    Up,
    Right,
    Down
} Direction;

typedef struct
{
    Sprite sprite;

    v2 pos;
    v2 vel;
    f32 speed;

    EntityType type;
    Direction dir;
    CollisionFlag collision_flag;

    PlateColor plate_color;

    b32 activated;

    b32 has_timer;
    f32 timer;
    f32 deactivation_time;

    f32 alpha;

    b32 collision_handled_in_this_frame;
} Entity;

