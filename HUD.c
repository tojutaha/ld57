#pragma once
#include "root.h"

function void
RenderHUD(GameState *gamestate)
{

    // NOTE: Debug HUD
#if INTERNAL
    Color bg_color = ColorAlpha(BLACK, 0.65f);
    Rectangle bg = (Rectangle){ 10, 10, 200, 200 };
    DrawRectangleRec(bg, bg_color);

    f32 text_y = 20.0f;
    DrawText(TextFormat("LEVEL: %d", gamestate->level_num), 20, text_y, 20, GREEN);
    text_y += 20.0f;
    DrawText(TextFormat("Zoom: %.2f", gamestate->camera.zoom), 20, text_y, 20, GREEN);
#endif
}

