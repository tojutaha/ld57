#pragma once
#include "root.h"

#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 64

typedef enum
{
    CollisionFlag_None    = 0,
    CollisionFlag_Overlap = 1 << 0,
    CollisionFlag_Block   = 1 << 1,
} CollisionFlag;

typedef enum
{
    Player,
    Door,
    Obstacle,
    PressurePlate,
} EntityType;

function const char *
PrettifyEntityType(EntityType type)
{
    switch(type)
    {
    case Player: return "Player";
    case Door: return "Door";
    case Obstacle: return "Obstacle";
    case PressurePlate: return "PressurePlate";
    default: return "Unknown";
    }
}

typedef enum
{
    Left,
    Right,
    Up,
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

    b32 pressure_plate_active;

} Entity;

