#pragma once
#include "root.h"

#define MAP_HEIGHT 13
#define MAP_WIDTH 13
#define TILE_WIDTH 64
#define TILE_HEIGHT 64

#define PRESSURE_PLATE_SIZE 64

#define MAX_ENTITIES 100
#define ENTITY_MAX_VELOCITY 200

typedef struct
{
    u32 active_entity_count;
    b32 door_open;

    u32 entity_count;
    Entity entities[MAX_ENTITIES];
} Tilemap;

