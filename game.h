#pragma once
#include "root.h"

typedef struct
{
    Entity player;

    Texture2D texture_atlas;

    Camera2D camera;

    s32 window_width;
    s32 window_height;

    u32 level_num;

} GameState;
