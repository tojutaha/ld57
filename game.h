#pragma once
#include "root.h"

typedef enum
{
    GameMode_Game,
    GameMode_MainMenu,
    GameMode_EndMenu
} GameMode;

typedef struct
{
    Entity player;

    Tilemap current_map;

    Texture2D texture_atlas;

    GameMode game_mode;

    Camera2D camera;
    f32 fade_alpha;
    b32 is_fading;
    b32 fade_in;
    f32 fade_timer;
    f32 fade_duration;
    b32 pending_level_increment;
    b32 player_can_move;

    s32 screen_width;
    s32 screen_height;

    u32 level_num;

} GameState;
