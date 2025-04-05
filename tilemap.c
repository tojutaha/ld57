#pragma once
#include "root.h"

function void
InitTilemap(Tilemap *map)
{
    map->door_open = false;

    for(u32 y = 0; y < MAP_HEIGHT; ++y)
    {
        for(u32 x = 0; x < MAP_WIDTH; ++x)
        {
            f32 tile_x = x * TILE_WIDTH;
            f32 tile_y = y * TILE_HEIGHT;

            if(x == MAP_WIDTH/2 && y == 0)
            {
                Entity door = { .pos = (v2){tile_x, tile_y}, Vector2Zero(), 0, Door, Left, CollisionFlag_Block };
                map->entities[map->entity_count++] = door;
                continue;
            }

            if(x == 0 || x == MAP_WIDTH-1 || y == 0 || y == MAP_HEIGHT-1)
            {
                Entity wall = { .pos = (v2){tile_x, tile_y}, Vector2Zero(), 0, Obstacle, Left, CollisionFlag_Block };
                map->entities[map->entity_count++] = wall;
            }
        }
    }
}

function void
DrawMapAndEntities(Tilemap *map)
{
    for(u32 y = 0; y < MAP_HEIGHT; ++y)
    {
        for(u32 x = 0; x < MAP_WIDTH; ++x)
        {
            f32 tile_x = x * TILE_WIDTH;
            f32 tile_y = y * TILE_HEIGHT;

            DrawRectangle(tile_x, tile_y, TILE_WIDTH, TILE_HEIGHT, GREEN);
            DrawRectangleLines(tile_x, tile_y, TILE_WIDTH, TILE_HEIGHT, BLACK);
        }
    }

    for(u32 entity_index = 0; entity_index < map->entity_count; ++entity_index)
    {
        Entity *e = map->entities + entity_index;
        switch(e->type)
        {
            case Obstacle:
            {
                DrawRectangleV(e->pos, (v2){ TILE_WIDTH, TILE_HEIGHT }, DARKGREEN);
            } break;

            case Door:
            {
                if(map->pressure_plate_count <= 0)
                {
                    map->door_open = true;
                    e->collision_flag = CollisionFlag_Overlap;
                }

                Color color = map->door_open ? GOLD : DARKGREEN;

                DrawRectangleV(e->pos, (v2){ TILE_WIDTH, TILE_HEIGHT }, color);
            } break;

            case PressurePlate:
            {
                Color color = e->pressure_plate_active ? RED : BLUE;
                DrawRectangleV(e->pos, (v2){ PRESSURE_PLATE_SIZE, PRESSURE_PLATE_SIZE }, color);
            }

            default: break;
        }
    }
}

function void
AddPressurePlate(Tilemap *map, s32 x, s32 y)
{
    Entity e =
    {
        // .pos = (v2){ x * TILE_WIDTH + PRESSURE_PLATE_SIZE * 0.5f, y * TILE_HEIGHT + PRESSURE_PLATE_SIZE * 0.5f},
        .pos = (v2){ x * TILE_WIDTH + PRESSURE_PLATE_SIZE, y * TILE_HEIGHT + PRESSURE_PLATE_SIZE },
        .vel = Vector2Zero(),
        .speed = 0,
        .type = PressurePlate,
        .collision_flag = CollisionFlag_Overlap,
        .pressure_plate_active = false,
    };

    map->entities[map->entity_count++] = e;
    map->pressure_plate_count++;
}

function void
IncrementLevel(GameState *gamestate)
{
    printf("NOT IMPLEMENTED!\n");
    gamestate->level_num++;
}
