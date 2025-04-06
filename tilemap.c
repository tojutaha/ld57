#pragma once
#include "root.h"

function b32
IsAllEntitiesActivated(Tilemap *map)
{
    b32 result = true;

    for(u32 i = 0; i < map->entity_count; ++i)
    {
        Entity *e = &map->entities[i];
        if(e->type == PressurePlate && !e->activated)
        {
            result = false;
            break;
        }
    }

    return result;
}

function void
UpdateAndDrawMapAndEntities(GameState *gamestate, Tilemap *map, f32 dt)
{
    for(u32 y = 0; y < MAP_HEIGHT; ++y)
    {
        for(u32 x = 0; x < MAP_WIDTH; ++x)
        {
            f32 tile_x = x * TILE_WIDTH;
            f32 tile_y = y * TILE_HEIGHT;

            DrawRectangle(tile_x, tile_y, TILE_WIDTH, TILE_HEIGHT, DARKGRAY);
            DrawRectangleLines(tile_x, tile_y, TILE_WIDTH, TILE_HEIGHT, BLACK);
        }
    }

    for(u32 entity_index = 0; entity_index < map->entity_count; ++entity_index)
    {
        Entity *e = map->entities + entity_index;
        switch(e->type)
        {
            case Wall:
            {
                DrawTextureRec(gamestate->texture_atlas, e->sprite.src, e->pos, WHITE);
            } break;

            case Door:
            {
                v2 target_pos = e->pos;

                map->door_open = IsAllEntitiesActivated(map);
                if(map->door_open)
                {
                    if(e->collision_flag & CollisionFlag_Block)
                        PlaySound(gamestate->door1);

                    e->collision_flag = CollisionFlag_Overlap;

                    target_pos.y -= 1.25f * TILE_HEIGHT;
                }
                else
                {
                    if(e->collision_flag & CollisionFlag_Overlap)
                        PlaySound(gamestate->door2);

                    e->collision_flag = CollisionFlag_Block;

                    target_pos = e->pos;
                }

                f32 speed = 2.0f;
                e->sprite.pos = Vector2Lerp(e->sprite.pos, target_pos, Clamp(dt*speed, 0.0f, 1.0f));
                DrawRectangleV(e->sprite.pos, (v2){ TILE_WIDTH, TILE_HEIGHT }, DARKBROWN);

            } break;

            case PressurePlate:
            {
                v2 original_size = (v2){ PRESSURE_PLATE_SIZE, PRESSURE_PLATE_SIZE };
                v2 size = original_size;
                f32 speed = 12.0f;
                f32 shrink_amount = 6.0f;

                if(e->has_timer && e->activated)
                {
                    e->timer += dt;
                    if(e->timer >= e->deactivation_time)
                    {
                        e->activated = false;
                        e->timer = 0.0f;
                        e->collision_flag = CollisionFlag_Overlap;
                        PlaySound(gamestate->blip2);
                    }
                }

                if(e->activated)
                {
                    e->sprite.src.x = 96;
                    v2 target = (v2){
                        PRESSURE_PLATE_SIZE - shrink_amount * 0.3f,
                        PRESSURE_PLATE_SIZE - shrink_amount
                    };
                    e->alpha += dt * speed;
                    e->alpha = Clamp(e->alpha, 0.0f, 1.0f);
                    size = Vector2Lerp(size, target, e->alpha);
                }
                else
                {
                    e->sprite.src.x = 32;
                    v2 target = (v2){
                        PRESSURE_PLATE_SIZE-shrink_amount * 0.3f,
                        PRESSURE_PLATE_SIZE-shrink_amount
                    };
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

                Rectangle dst =
                {
                    draw_pos.x, draw_pos.y, size.x, size.y
                };

                DrawTexturePro(gamestate->texture_atlas, e->sprite.src, dst, (v2){0, 0}, 0.0f, WHITE);
            } break;

            default: break;
        }
    }
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
                AddDoor(map, tile_x, tile_y);
                continue;
            }

            if(x == 0 || x == MAP_WIDTH-1 || y == 0 || y == MAP_HEIGHT-1)
            {
                AddWall(map, tile_x, tile_y);
            }
        }
    }
}


function void
ResetTilemap(Tilemap *map)
{
    map->active_entity_count = 0;
    memset(map->entities, 0, sizeof(Entity) * MAX_ENTITIES);
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
            // Default first level; Learn about the plates and door
            AddPressurePlate(&gamestate->current_map, 1, 1);

            return true;
        }

        case 1:
        {
            // Little variation for next level; Learn about the timers
            AddPressurePlate(&gamestate->current_map, 1, 1);
            AddPressurePlate(&gamestate->current_map, 2, 2);
            Entity * e = AddPressurePlate(&gamestate->current_map, 3, 3);
            e->has_timer = true;
            e->deactivation_time = 2.5f;

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

function void
NewGame(GameState *gamestate)
{
    gamestate->fade_timer = 0;
    gamestate->level_num = 0;
    SetupLevel(gamestate, gamestate->level_num);

    gamestate->player = AddPlayer(gamestate);
    gamestate->player_can_move = true;
}

