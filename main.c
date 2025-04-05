#pragma once
#include "root.h"

int main(void)
{
    GameState gamestate = {0};
    Tilemap map = {0};

    InitGame(&gamestate, &map);

    Entity player = AddPlayer(&gamestate);

    while(!WindowShouldClose())
    {
        f32 dt = GetFrameTime();
        UpdateGame(&gamestate, &map, dt);
        RenderGame(&gamestate, &map, dt);
    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
