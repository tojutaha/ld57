#pragma once
#include "root.h"


typedef enum
{
    AnimState_Idle,
    AnimState_Walk,
} AnimState;

typedef struct
{
    Rectangle src;
    v2 pos;

    // Animation
    struct
    {
        u16 frame_counter : 5;
        u16 current_frame : 5;
        u16 reserved      : 6;
    };

    enum16(AnimState) anim_state;

} Sprite;

