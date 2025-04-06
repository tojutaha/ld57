#pragma once
#include "root.h"

int main(void)
{
    GameState *gamestate = malloc(sizeof *gamestate);
    Assert(gamestate != NULL);
    memset(gamestate, 0, sizeof *gamestate);

    InitGame(gamestate);
    SetExitKey(0);

    // while(!WindowShouldClose())
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
                if(IsKeyDown(KEY_UP)) gamestate->camera.rotation += 0.1f;
                if(IsKeyDown(KEY_DOWN)) gamestate->camera.rotation -= 0.1f;
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
    CloseAudioDevice();
    CloseWindow();
    free(gamestate);

    return 0;
}
