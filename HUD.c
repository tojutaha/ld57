#pragma once
#include "root.h"

function void
RenderHUD(GameState *gamestate)
{
    // TODO: Might be reduntant..
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

function void
DrawCameraFade(GameState *gamestate)
{
    if(gamestate->is_fading || gamestate->fade_alpha > 0.0f)
    {
        Color color = BLACK;
        color.a = (u8)(gamestate->fade_alpha * 255);
        DrawRectangle(0, 0, gamestate->screen_width, gamestate->screen_height, color);
    }
}

function void
DrawMainMenu(GameState *gamestate)
{
    BeginDrawing();
    ClearBackground(BLACK);

    v2 mouse_pos = GetMousePosition();

    Button new_game_button =
    {
        .pos = (v2){ 10, 10 },
        .text = "NEW GAME",
        .width = 150,
        .height = 50,
        .normal_color = BLUE,
        .hover_color = RED
    };

    if(ButtonPressed(&new_game_button, mouse_pos))
    {
        gamestate->game_mode = GameMode_Game;
        NewGame(gamestate);
    }

    Button exit_button = new_game_button;
    exit_button.pos.y += 75;
    exit_button.text = "EXIT GAME";

    if(ButtonPressed(&exit_button, mouse_pos))
    {
        global_running = false;
    }

    EndDrawing();
}

function void
DrawEndMenu(GameState *gamestate)
{
    BeginDrawing();
    ClearBackground(BLACK);

    v2 mouse_pos = GetMousePosition();

    DrawText("Yep, thats all levels.. for now.", 10, 20, 20, WHITE);

    Button exit_button =
    {
        .pos = (v2){ 10, 50 },
        .text = "MAIN MENU",
        .width = 150,
        .height = 50,
        .normal_color = BLUE,
        .hover_color = RED
    };

    if(ButtonPressed(&exit_button, mouse_pos))
    {
        gamestate->game_mode = GameMode_MainMenu;
    }

    EndDrawing();
}
