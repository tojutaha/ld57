#pragma once
#include "root.h"

typedef enum
{
    Axis_X,
    Axis_Y,
} Axis;

typedef struct
{
    Entity *collided_entity;
    v2 normal;
    b32 blocking;
    b32 overlapping;
} CollisionResult;

