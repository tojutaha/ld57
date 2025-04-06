#pragma once
#include "root.h"

function void
DrawMapAndEntities(Tilemap *map, f32 dt)
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
                b32 all_presssed = true;
                for(u32 i = 0; i < map->entity_count; ++i)
                {
                    Entity *entity = &map->entities[i];
                    if(entity->type == PressurePlate && !entity->pressure_plate_active)
                    {
                        all_presssed = false;
                        break;
                    }
                }

                map->door_open = all_presssed;
                if(map->door_open)
                    e->collision_flag = CollisionFlag_Overlap;

                Color color = map->door_open ? GOLD : DARKGREEN;

                DrawRectangleV(e->pos, (v2){ TILE_WIDTH, TILE_HEIGHT }, color);
            } break;

            case PressurePlate:
            {
                Color color = e->pressure_plate_active ? RED : BLUE;
                v2 original_size = (v2){ PRESSURE_PLATE_SIZE, PRESSURE_PLATE_SIZE };
                v2 size = original_size;
                f32 speed = 12.0f;
                f32 shrink_amount = 6.0f;

                if(e->has_timer && e->pressure_plate_active)
                {
                    e->pressure_plate_timer -= dt;
                    if(e->pressure_plate_timer <= 0)
                    {
                        e->pressure_plate_active =   false;
                        e->pressure_plate_timer = 3.0f;
                        e->collision_flag = CollisionFlag_Overlap;
                    }
                }

                if(e->pressure_plate_active)
                {
                    v2 target = (v2){ PRESSURE_PLATE_SIZE-shrink_amount, PRESSURE_PLATE_SIZE-shrink_amount };
                    e->alpha += dt * speed;
                    e->alpha = Clamp(e->alpha, 0.0f, 1.0f);
                    size = Vector2Lerp(size, target, e->alpha);
                }
                else
                {
                    v2 target = (v2){ PRESSURE_PLATE_SIZE-shrink_amount, PRESSURE_PLATE_SIZE-shrink_amount };
                    e->alpha -= dt * speed;
                    e->alpha = Clamp(e->alpha, 0.0f, 1.0f);
                    size = Vector2Lerp(size, target, e->alpha);
                }

                v2 offset =
                {
                    (original_size.x - size.x) * 0.5f,
                    (original_size.y - size.y) * 0.5f,
                };

                v2 draw_pos = Vector2Add(e->pos, offset);

                DrawRectangleV(draw_pos, size, color);
            } break;

            default: break;
        }
    }
}

function Entity *
AddPressurePlate(Tilemap *map, s32 x, s32 y)
{
    Entity e =
    {
        .pos = (v2){ x * TILE_WIDTH + PRESSURE_PLATE_SIZE, y * TILE_HEIGHT + PRESSURE_PLATE_SIZE },
        .vel = Vector2Zero(),
        .speed = 0,
        .type = PressurePlate,
        .collision_flag = CollisionFlag_Overlap,
        .pressure_plate_active = false,
        .has_timer = false,
        .pressure_plate_timer = 0,
    };

    map->entities[map->entity_count++] = e;
    map->pressure_plate_count++;

    return &map->entities[map->entity_count - 1];
}

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
ResetTilemap(Tilemap *map)
{
    map->pressure_plate_count = 0;
    memset(map->entities, 0, sizeof(Entity) * /*map->entity_count*/ MAX_ENTITIES);
    map->entity_count = 0;
    map->door_open = false;
}

function void
StartLevelFade(GameState *gamestate, u32 next_level)
{
    gamestate->fade_timer = 0;
    gamestate->fade_duration = 0.5f;
    gamestate->fade_in = false;
    gamestate->is_fading = true;
    gamestate->player_can_move = false;

    gamestate->pending_level_increment = true;
    gamestate->level_num = next_level;
}

function b32
SetupLevel(GameState *gamestate, u32 level_num)
{
    ResetTilemap(&gamestate->current_map);
    InitTilemap(&gamestate->current_map);

    switch(level_num)
    {
        case 0:
        {
            // Default first level
            AddPressurePlate(&gamestate->current_map, 1, 1);

            return true;
        }

        case 1:
        {
            // Little variation for next level
            AddPressurePlate(&gamestate->current_map, 1, 1);
            AddPressurePlate(&gamestate->current_map, 2, 2);
            Entity * e = AddPressurePlate(&gamestate->current_map, 3, 3);
            e->has_timer = true;
            e->pressure_plate_timer = 3.0f;

            return true;
        }

        // TODO: Implement more levels

        default: return false;
    }
}

function void
IncrementLevel(GameState *gamestate)
{
    StartLevelFade(gamestate, gamestate->level_num + 1);
}
