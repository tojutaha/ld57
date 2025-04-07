#pragma once
#include "root.h"

typedef struct
{
    v2 pos;
    const char *text;
    f32 width, height;
    Color normal_color;
    Color hover_color;
} Button;

typedef enum
{
    ButtonStateNormal = 0,
    ButtonStateMouseHover,
    ButtonStatePressed,
} ButtonState;

function b32
ButtonPressed(Button *button, v2 mouse_pos)
{
    Rectangle button_rect = { button->pos.x, button->pos.y, button->width, button->height };
    ButtonState button_state = ButtonStateNormal;
    b32 button_action = false;

    if(CheckCollisionPointRec(mouse_pos, button_rect))
    {
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            button_state = ButtonStatePressed;
        else
            button_state = ButtonStateMouseHover;

        if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            button_action = true;
    }
    else
        button_state = ButtonStateNormal;

    if(button_action)
    {
        button_action = !button_action;
    }

    Color button_color = button->normal_color;
    if(button_state == ButtonStateMouseHover)
        button_color = button->hover_color;
    if(button_state == ButtonStatePressed)
        button_color = button->normal_color;

    // Draw button rectangle
    DrawRectangleRec(button_rect, button_color);
    
    // Calculate text position to center it
    const s32 font_size = 20;
    Font font = GetFontDefault();
    v2 text_size = MeasureTextEx(font, button->text, font_size, 2);

    v2 text_pos = {
        button->pos.x + (button->width - text_size.x) / 2,
        button->pos.y + (button->height - text_size.y) / 2
    };

    // Draw button text
    DrawTextEx(font, button->text, text_pos, font_size, 2, WHITE);

    return (button_state == ButtonStatePressed) ? true : false;
}
