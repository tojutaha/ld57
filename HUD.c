#pragma once
#include "root.h"

function void
RenderHUD(GameState *gamestate)
{
    DrawText(TextFormat("LEVEL: %d", gamestate->level_num+1), 20, 20, 20, GREEN);
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

#define button_width 150
#define button_height 50
#define button_spacing 25

function void
DrawMainMenu(GameState *gamestate)
{
    BeginDrawing();
    ClearBackground(BLACK);

    v2 mouse_pos = GetMousePosition();

    f32 center_x = gamestate->screen_width * 0.5f;
    f32 button_center_x = (gamestate->screen_width - button_width) * 0.5f;
    f32 total_height = button_height * 2 + button_spacing;
    f32 start_y = (gamestate->screen_height - total_height) * 0.5f;

    const char *text = "TITLE";
    f32 font_size = 40;
    f32 text_width = MeasureText(text, font_size);
    DrawText(text, center_x - text_width*0.5f, start_y - button_width, font_size, WHITE);

    Button new_game_button =
    {
        .pos = (v2){ button_center_x, start_y },
        .text = "NEW GAME",
        .width = button_width,
        .height = button_height,
        .normal_color = BLUE,
        .hover_color = RED
    };

    if(ButtonPressed(&new_game_button, mouse_pos))
    {
        gamestate->game_mode = GameMode_Game;
        NewGame(gamestate);
    }

    Button exit_button = new_game_button;
    exit_button.pos.y += button_height + button_spacing;
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

    f32 center_x = gamestate->screen_width * 0.5f;
    f32 button_center_x = (gamestate->screen_width - button_width) * 0.5f;
    f32 total_height = button_height + button_spacing;
    f32 start_y = (gamestate->screen_height - total_height) * 0.5f;

    const char *text = "Yep, thats all levels..";
    f32 font_size = 40;
    f32 text_width = MeasureText(text, font_size);
    DrawText(text, center_x - text_width*0.5f, start_y - button_width, font_size, WHITE);

    Button exit_button =
    {
        .pos = (v2){ button_center_x, start_y },
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
