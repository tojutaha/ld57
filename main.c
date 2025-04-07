#pragma once
#include "root.h"

int main(void)
{
    GameState *gamestate = malloc(sizeof *gamestate);
    Assert(gamestate != NULL);
    memset(gamestate, 0, sizeof *gamestate);

    InitGame(gamestate);

#if !INTERNAL
    SetExitKey(0);
#endif

    global_running = true;
    while(global_running)
    {
        global_running =  !WindowShouldClose();

        f32 dt = GetFrameTime();

        switch(gamestate->game_mode)
        {
            case GameMode_Game:
            {
#if INTERNAL
                if(IsKeyPressed(KEY_UP))
                    IncrementLevel(gamestate);
#endif
                if(IsKeyPressed(KEY_ESCAPE))
                    gamestate->game_mode = GameMode_MainMenu;

                UpdateGame(gamestate, dt);
                RenderGame(gamestate, dt);
            } break;

            case GameMode_MainMenu:
            {
                DrawMainMenu(gamestate);
            } break;

            case GameMode_EndMenu:
            {
                DrawEndMenu(gamestate);
            } break;
        }

    }

    UnloadTexture(gamestate->texture_atlas);
    free(gamestate);

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
