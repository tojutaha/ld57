#pragma once
#include "root.h"

#define MAP_HEIGHT 12
#define MAP_WIDTH 12
#define TILE_WIDTH 64
#define TILE_HEIGHT 64

#define MAX_ENTITIES 100
#define ENTITY_MAX_VELOCITY 200

typedef struct
{
    u32 pressure_plate_count;
    b32 door_open;

    u32 entity_count;
    Entity entities[MAX_ENTITIES];
} Tilemap;

