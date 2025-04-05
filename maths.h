#pragma once
#include "root.h"

function inline f32
EaseInOut(f32 t)
{
    return t < 0.5f ? 4.0f * t * t * t : 1.0f - powf(-2.0f * t + 2.0f, 3.0f) / 2.0f;
}

function inline f32
EaseIn(f32 t)
{
    return t * t;
}

function inline f32
EaseOut(f32 t)
{
    return 1.0f - powf(1.0f - t, 3.0f);
}

