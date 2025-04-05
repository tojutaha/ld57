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

        switch(gamestate->game_mode)
        {
            case GameMode_Game:
            {
                UpdateGame(gamestate, dt);
                RenderGame(gamestate, dt);
            } break;

            case GameMode_MainMenu:
            {
                DrawMainMenu(gamestate);
            } break;

            case GameMode_PauseMenu:
            {
                DrawPauseMenu(gamestate);
            } break;
        }

    }

    UnloadTexture(gamestate->texture_atlas);
    CloseAudioDevice();
    CloseWindow();
    free(gamestate);

    return 0;
}
