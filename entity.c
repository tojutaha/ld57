#pragma once
#include "root.h"

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

    v2 start_pos = { gamestate->screen_width / 2.0f, gamestate->screen_height / 2.0f };
    Entity player = { sprite, start_pos, Vector2Zero(), 2000, Player, Left, CollisionFlag_Overlap };
    sprite.pos = player.pos;

    gamestate->camera.rotation = 0.0f;
    gamestate->camera.offset = start_pos;
    gamestate->camera.zoom = 1.5f;
    gamestate->camera.target = player.pos;

    return player;
}

function void
ResetPlayerPos(GameState *gamestate)
{
    v2 start_pos = { gamestate->screen_width / 2.0f, gamestate->screen_height / 2.0f };
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
