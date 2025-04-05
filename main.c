#pragma once
#include "root.h"

int main(void)
{
    GameState *gamestate = malloc(sizeof *gamestate);
    Assert(gamestate != NULL);

    InitGame(gamestate);

    while(!WindowShouldClose())
    {
        f32 dt = GetFrameTime();
        UpdateGame(gamestate, dt);
        RenderGame(gamestate, dt);
    }

    UnloadTexture(gamestate->texture_atlas);
    CloseAudioDevice();
    CloseWindow();
    free(gamestate);

    return 0;
}
