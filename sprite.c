#pragma once
#include "root.h"

function void
DrawSprite(Texture2D texture, Sprite sprite, u32 width, u32 height)
{
    const Rectangle dst = {
        sprite.pos.x,
        sprite.pos.y,
        width,
        height
    };

    DrawTexturePro(texture, sprite.src, dst, (v2){0, 0}, 0.0f, WHITE);
}

