#pragma once
#include "root.h"

//
// Add entities
//

#define PLAYER_SPEED 2000
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
    Entity player = { sprite, start_pos, Vector2Zero(), PLAYER_SPEED, Player, Left, CollisionFlag_Overlap };
    sprite.pos = player.pos;

    gamestate->camera.rotation = 0.0f;
    gamestate->camera.offset = start_pos;
    gamestate->camera.zoom = CAMERA_ZOOM;
    gamestate->camera.target = player.pos;

    return player;
}

function void
AddClone(Tilemap *map, s32 x, s32 y)
{
    v2 pos = { x * TILE_WIDTH, y * TILE_HEIGHT };
    Sprite sprite =
    {
        .src = (Rectangle){ 0, 256, 32, 64 },
        .anim_state = AnimState_Idle,
        .current_frame = 0,
        .frame_counter = 0,
        .pos = pos,
    };

    Entity e =
    {
        .pos = pos,// (v2){ x, y },
        .vel = Vector2Zero(),
        .speed = 0,
        .type = Clone,
        .collision_flag = CollisionFlag_None, // No need to have a flag,
                                              // we just loop over the pressure plates in
                                              // MoveClone
        .activated = false,
        .has_timer = false,
        .deactivation_time = 0,
        .timer = 0,
        .sprite = sprite
    };

    e.speed = 1;

    map->entities[map->entity_count++] = e;
}

function void
AddCloneV(Tilemap *map, v2 pos)
{
    Sprite sprite =
    {
        .src = (Rectangle){ 0, 256, 32, 64 },
        .anim_state = AnimState_Idle,
        .current_frame = 0,
        .frame_counter = 0,
        .pos = pos,
    };

    Entity e =
    {
        .pos = pos,// (v2){ x, y },
        .vel = Vector2Zero(),
        .speed = 0,
        .type = Clone,
        .collision_flag = CollisionFlag_None, // No need to have a flag,
                                              // we just loop over the pressure plates in
                                              // MoveClone
        .activated = false,
        .has_timer = false,
        .deactivation_time = 0,
        .timer = 0,
        .sprite = sprite
    };

    e.speed = 1;

    map->entities[map->entity_count++] = e;
}

function Entity *
AddPressurePlate(Tilemap *map, s32 x, s32 y, PlateColor color)
{
    Sprite sprite = {0};

    switch(color)
    {
        case PlateColor_Red:
        {
            sprite.src = (Rectangle){ 160, 320, PRESSURE_PLATE_SIZE, PRESSURE_PLATE_SIZE };
        } break;

        case PlateColor_Green:
        {
            sprite.src = (Rectangle){ 160, 384, PRESSURE_PLATE_SIZE, PRESSURE_PLATE_SIZE };
        } break;

        case PlateColor_Blue:
        {
            sprite.src = (Rectangle){ 160, 448, PRESSURE_PLATE_SIZE, PRESSURE_PLATE_SIZE };
        } break;

        case PlateColor_Yellow:
        {
            sprite.src = (Rectangle){ 160, 512, PRESSURE_PLATE_SIZE, PRESSURE_PLATE_SIZE };
        } break;

        case PlateColor_Purple:
        {
            sprite.src = (Rectangle){ 160, 576, PRESSURE_PLATE_SIZE, PRESSURE_PLATE_SIZE };
        } break;

        case PlateColor_White:
        {
            sprite.src = (Rectangle){ 160, 640, PRESSURE_PLATE_SIZE, PRESSURE_PLATE_SIZE };
        } break;

        case PlateColor_Orange:
        {
            sprite.src = (Rectangle){ 160, 704, PRESSURE_PLATE_SIZE, PRESSURE_PLATE_SIZE };
        } break;

        case PlateColor_Cyan:
        {
            sprite.src = (Rectangle){ 160, 768, PRESSURE_PLATE_SIZE, PRESSURE_PLATE_SIZE };
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
        .plate_color = color,
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
    sprite.src = (Rectangle){ 192, 64, TILE_WIDTH, TILE_HEIGHT };
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
    sprite.src = (Rectangle){ 384, 64, TILE_WIDTH, TILE_HEIGHT };
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

function void
AddBeamEmitter(Tilemap *map, s32 x, s32 y, Direction dir)
{
    v2 pos = { x * TILE_WIDTH, y * TILE_HEIGHT };
    // TODO Sprite

    Entity e =
    {
        .pos = pos,
        .vel = Vector2Zero(),
        .speed = 0,
        .type = BeamEmitter,
        .collision_flag = CollisionFlag_Overlap,
        .activated = false,
        .has_timer = false,
        .deactivation_time = 0,
        .timer = 0,
        .dir = dir
        // .sprite = sprite
    };

    map->entities[map->entity_count++] = e;
    map->active_entity_count++;
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
AnimatePlayer(Entity *e)
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

    // @HARCODED
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

//
// Clone
//

function void
MoveClone(GameState *gamestate, Entity *clone, Entity *player, f32 dt)
{
    Tilemap *map = &gamestate->current_map;

    // Keep the camera in the middle until door is open
    if(!gamestate->current_map.door_open)
    {
        v2 mid_point =
        {
            (player->pos.x + clone->pos.x) * 0.5f,
            (player->pos.y + clone->pos.y) * 0.5f
        };
        gamestate->camera.target = mid_point;
    }

    // Mirror the player vel/pos 
    clone->vel.x = -player->vel.x;
    clone->vel.y = -player->vel.y;

    clone->pos.x += clone->vel.x * clone->speed * dt;
    clone->pos.y += clone->vel.y * clone->speed * dt;

    clone->sprite.pos = clone->pos;
    clone->dir = player->dir;
    clone->sprite.anim_state = player->sprite.anim_state;

    // Check collision, we dont care anything but the specific pressure plates
    Rectangle clone_rect =
    {
        clone->pos.x,
        clone->pos.y + PLAYER_WIDTH,
        PLAYER_WIDTH,
        PLAYER_HEIGHT*0.5f
    };

    for(u32 i = 0; i < map->entity_count; ++i)
    {
        Entity *e = &map->entities[i];
        if(e->type == PressurePlate && e->plate_color == PlateColor_Blue)
        {
            Rectangle entity_rect = (Rectangle){ e->pos.x, e->pos.y, PRESSURE_PLATE_SIZE-10, PRESSURE_PLATE_SIZE-10 };

            if(CheckCollisionRecs(clone_rect, entity_rect))
            {
                if(!e->activated)
                {
                    e->activated = true;
                    e->collision_flag = CollisionFlag_None;
                    PlaySound(gamestate->blip1);
                }
            }
        }
    }
}

function void
AnimateClone(Entity *e)
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

    // @HARCODED
    switch(e->sprite.anim_state)
    {
        case AnimState_Idle:
        {
            if(e->dir == Right || e->dir == Up) e->sprite.src.y = 256;
            else if(e->dir == Left || e->dir == Down) e->sprite.src.y = 320;
            e->sprite.src.x = (f32)e->sprite.current_frame * (f32)PLAYER_WIDTH;
        } break;

        case AnimState_Walk:
        {
            if(e->dir == Right || e->dir == Up) e->sprite.src.y = 384;
            else if(e->dir == Left || e->dir == Down) e->sprite.src.y = 448;
            e->sprite.src.x = (f32)e->sprite.current_frame * (f32)PLAYER_WIDTH;
        } break;

        default: break;
    }
}
