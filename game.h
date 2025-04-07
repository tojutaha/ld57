#pragma once
#include "root.h"

#define LAST_LEVEL 9
#define MAX_SEQUENCE_LEN 8

typedef struct
{
    PlateColor color_sequence[MAX_SEQUENCE_LEN];
    u32 sequence_len;
    u32 current_index;
    b32 sequence_failed;
    b32 sequence_completed;
} PlateSequence;

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

    Sound blip1;
    Sound blip2;
    Sound door1;
    Sound door2;

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

    PlateSequence plate_sequence;

} GameState;

function void
InitColorSequence(PlateSequence *seq, PlateColor *colors, u32 count)
{
    seq->sequence_len = count;
    seq->current_index = 0;
    seq->sequence_failed = false;
    seq->sequence_completed = false;
    for(u32 i = 0; i < count; ++i)
        seq->color_sequence[i] = colors[i];
}
