#pragma once
#include "root.h"

int main(void)
{
    GameState gamestate = {0};
    InitGame(&gamestate);

    while(!WindowShouldClose())
    {
        f32 dt = GetFrameTime();
        UpdateGame(&gamestate, dt);
        RenderGame(&gamestate, dt);
    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
