#pragma once
#include "root.h"

//
// Add entities
//

function Entity
AddPlayer(GameState *gamestate)
{
    Sprite sprite =
    {
        .src = (Rectangle){ 0, 0, 32, 64 },
        .anim_state = AnimState_Idle,
        .current_frame = 0,
        .frame_counter = 0,
    };

    v2 start_pos = { (TILE_WIDTH * MAP_WIDTH) / 2.0f, (TILE_HEIGHT * MAP_HEIGHT) / 2.0f };
    Entity player = { sprite, start_pos, Vector2Zero(), 2000, Player, Left, CollisionFlag_Overlap };
    sprite.pos = player.pos;

    gamestate->camera.rotation = 0.0f;
    gamestate->camera.offset = start_pos;
    gamestate->camera.zoom = CAMERA_ZOOM;
    gamestate->camera.target = player.pos;

    return player;
}

function Entity *
AddPressurePlate(Tilemap *map, s32 x, s32 y, PlateColor color)
{
    Sprite sprite = {0};

    switch(color)
    {
        case PlateColor_Red:
        {
            sprite.src = (Rectangle){ 32, 320, PRESSURE_PLATE_SIZE, PRESSURE_PLATE_SIZE };
        } break;

        case PlateColor_Green:
        {
            sprite.src = (Rectangle){ 32, 384, PRESSURE_PLATE_SIZE, PRESSURE_PLATE_SIZE };
        } break;

        case PlateColor_Blue:
        {
            sprite.src = (Rectangle){ 32, 448, PRESSURE_PLATE_SIZE, PRESSURE_PLATE_SIZE };
        } break;
    }

    Entity e =
    {
        .pos = (v2){ x * TILE_WIDTH + PRESSURE_PLATE_SIZE, y * TILE_HEIGHT + PRESSURE_PLATE_SIZE },
        .vel = Vector2Zero(),
        .speed = 0,
        .type = PressurePlate,
        .collision_flag = CollisionFlag_Overlap,
        .activated = false,
        .has_timer = false,
        .deactivation_time = 0,
        .timer = 0,
        .sprite = sprite
    };

    map->entities[map->entity_count++] = e;
    map->active_entity_count++;

    return &map->entities[map->entity_count - 1];
}

function void
AddWall(Tilemap *map, s32 x, s32 y)
{
    Sprite sprite = {0};
    sprite.src = (Rectangle){ 32, 576, TILE_WIDTH, TILE_HEIGHT };
    Entity e =
    {
        .pos = (v2){ x, y },
        .vel = Vector2Zero(),
        .speed = 0,
        .type = Wall,
        .collision_flag = CollisionFlag_Block,
        .activated = false,
        .has_timer = false,
        .deactivation_time = 0,
        .timer = 0,
        .sprite = sprite
    };

    map->entities[map->entity_count++] = e;
}

function void
AddDoor(Tilemap *map, s32 x, s32 y)
{
    v2 pos = { x, y };
    Sprite sprite = {0};
    sprite.src = (Rectangle){ 224, 576, TILE_WIDTH, TILE_HEIGHT };
    sprite.pos = pos;

    Entity e =
    {
        .pos = pos,
        .vel = Vector2Zero(),
        .speed = 0,
        .type = Door,
        .collision_flag = CollisionFlag_Block,
        .activated = false,
        .has_timer = false,
        .deactivation_time = 0,
        .timer = 0,
        .sprite = sprite
    };

    map->entities[map->entity_count++] = e;
}

//
//
//

function void
ResetPlayerPos(GameState *gamestate)
{
    v2 start_pos = { (TILE_WIDTH * MAP_WIDTH) / 2.0f, (TILE_HEIGHT * MAP_HEIGHT) / 2.0f };
    gamestate->player.pos = start_pos;
    gamestate->player.sprite.pos = gamestate->player.pos;
}

function void
AnimateEntity(Entity *e)
{
    f32 anim_speed = 3.0f;
    if(e->sprite.frame_counter++ >= TARGET_FPS / anim_speed)
    {
        e->sprite.frame_counter = 0;
        e->sprite.current_frame++;

        if(e->sprite.current_frame > 3)
        {
            e->sprite.current_frame = 0;
        }
    }

    switch(e->sprite.anim_state)
    {
        case AnimState_Idle:
        {
            if(e->dir == Right || e->dir == Up) e->sprite.src.y = 0;
            else if(e->dir == Left || e->dir == Down) e->sprite.src.y = 64;
            e->sprite.src.x = (f32)e->sprite.current_frame * (f32)PLAYER_WIDTH;
        } break;

        case AnimState_Walk:
        {
            if(e->dir == Right || e->dir == Up) e->sprite.src.y = 128;
            else if(e->dir == Left || e->dir == Down) e->sprite.src.y = 192;
            e->sprite.src.x = (f32)e->sprite.current_frame * (f32)PLAYER_WIDTH;
        } break;

        default: break;
    }
}
