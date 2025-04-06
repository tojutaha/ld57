#pragma once
#include "root.h"

function void
InitGame(GameState *gamestate)
{
    gamestate->screen_width = 800;
    gamestate->screen_height = 600;

    SetConfigFlags(FLAG_VSYNC_HINT|FLAG_MSAA_4X_HINT);
    InitWindow(gamestate->screen_width, gamestate->screen_height, "LD57");
    SetTargetFPS(TARGET_FPS);

    InitAudioDevice();

    gamestate->texture_atlas = LoadTexture("./resources/spritesheet.png");
    SetTextureFilter(gamestate->texture_atlas, TEXTURE_FILTER_POINT);

    gamestate->blip1 = LoadSound("./resources/blip1.wav");
    gamestate->blip2 = LoadSound("./resources/blip2.wav");
    gamestate->door1 = LoadSound("./resources/door1.wav");
    gamestate->door2 = LoadSound("./resources/door2.wav");

#if INTERNAL
    gamestate->game_mode = GameMode_Game;
#else
    gamestate->game_mode = GameMode_MainMenu;
#endif
    
    NewGame(gamestate);
}

function void
UpdateGame(GameState *gamestate, f32 dt)
{
    Entity *player = &gamestate->player;
    Tilemap *map = &gamestate->current_map;

    v2 movement_vector = {0};

    if(gamestate->player_can_move)
    {
        if(IsKeyDown(KEY_A)) movement_vector.x -= 1;
        if(IsKeyDown(KEY_D)) movement_vector.x += 1;
        if(IsKeyDown(KEY_W)) movement_vector.y -= 1;
        if(IsKeyDown(KEY_S)) movement_vector.y += 1;
    }

    f32 len = Vector2LengthSqr(movement_vector);
    if(len > 0)
    {
        player->sprite.anim_state = AnimState_Walk;

        movement_vector.x *= 1.0f / sqrtf(len);
        movement_vector.y *= 1.0f / sqrtf(len);

        v2 direction_normalize = Vector2Normalize(movement_vector);

        if(direction_normalize.x < 0) player->dir = Left;
        if(direction_normalize.x > 0) player->dir = Right;
        if(direction_normalize.y < 0) player->dir = Up;
        if(direction_normalize.y > 0) player->dir = Down;
    }
    else
    {
        player->sprite.anim_state = AnimState_Idle;
    }

    movement_vector.x *= player->speed;
    movement_vector.y *= player->speed;

    f32 vel_len = Vector2Length(player->vel);
    if(vel_len > ENTITY_MAX_VELOCITY)
    {
        player->vel.x *= ENTITY_MAX_VELOCITY / vel_len;
        player->vel.y *= ENTITY_MAX_VELOCITY / vel_len;
    }

    movement_vector.x += -12.0f * player->vel.x;
    movement_vector.y += -12.0f * player->vel.y;

    v2 player_delta = {
        0.5f * movement_vector.x * (dt*dt) + player->vel.x * dt,
        0.5f * movement_vector.y * (dt*dt) + player->vel.y * dt,
    };

    player->vel.x += movement_vector.x * dt;
    player->vel.y += movement_vector.y * dt;

    // @Hack to avoid tunneling..
    player->pos.x += player_delta.x;
    CollisionResult collision_result_x = CheckCollision(map, player, &player->vel, Axis_X);

    player->pos.y += player_delta.y;
    CollisionResult collision_result_y = CheckCollision(map, player, &player->vel, Axis_Y);

#if 0
    if(collision_result_x.blocking && collision_result_x.collided_entity)
    {
        printf("Blocking collision with entity %s\n", PrettifyEntityType(collision_result_x.collided_entity->type));
    }
    if(collision_result_y.blocking && collision_result_y.collided_entity)
    {
        printf("Blocking collision with entity %s\n", PrettifyEntityType(collision_result_y.collided_entity->type));
    }
#endif

    if(collision_result_x.overlapping)
    {
        Entity *e = collision_result_x.collided_entity;
        HandleOverlappingCollision(gamestate, map, collision_result_x.collided_entity);
    }

    if(collision_result_y.overlapping)
    {
        Entity *e = collision_result_y.collided_entity;
        HandleOverlappingCollision(gamestate, map, collision_result_y.collided_entity);
    }

    player->sprite.pos = player->pos;

    // Update camera
    // gamestate->camera.target = player->pos;
    f32 min_speed = 20.0f;
    f32 min_effect = 20;
    f32 fraction_speed = 1.6f;

    gamestate->camera.offset = (v2){ gamestate->screen_width*0.5f, gamestate->screen_height*0.5f };
    v2 diff = Vector2Subtract(player->pos, gamestate->camera.target);
    f32 effect_len = Vector2Length(diff);
    if(effect_len > min_effect)
    {
        f32 speed = fmaxf(fraction_speed*effect_len, min_speed);
        gamestate->camera.target = Vector2Add(gamestate->camera.target, Vector2Scale(diff, speed*dt/effect_len));
    }

    // Update camera fade
    if(gamestate->is_fading)
    {
        f32 speed = 1.0f;
        gamestate->fade_timer += dt * speed;
        f32 t = Clamp(gamestate->fade_timer / gamestate->fade_duration, 0.0f, 1.0f);
        f32 eased = EaseInOut(t);
        gamestate->fade_alpha = gamestate->fade_in ? 1.0f - eased : eased;

        // Zoom the camera in a bit
        f32 camera_zoom_speed = 0.35f;
        gamestate->camera.zoom -= t * camera_zoom_speed;
        gamestate->camera.zoom = Clamp(gamestate->camera.zoom, 0.0, 1.0f);
        // And rotate, why not..
        gamestate->camera.rotation += t * 25.0f;

        if(t >= 1.0f)
        {
            if(!gamestate->fade_in)
            {
                // Finished fading
                if(gamestate->pending_level_increment)
                {
                    if(SetupLevel(gamestate, gamestate->level_num))
                    {
                        ResetPlayerPos(gamestate);
                    }
                    else
                    {
                        gamestate->game_mode = GameMode_EndMenu;
                    }

                    gamestate->pending_level_increment = false;
                }

                gamestate->fade_in = true;
                gamestate->fade_timer = 0;
            }
            else
            {
                // Done
                gamestate->camera.zoom = CAMERA_ZOOM;
                gamestate->camera.rotation = 0;
                gamestate->is_fading = false;
                gamestate->player_can_move = true;
            }
        }
    }
}

function void
RenderGame(GameState *gamestate, f32 dt)
{
    BeginDrawing();
    ClearBackground(BLACK);
    BeginMode2D(gamestate->camera);

    Tilemap *map = &gamestate->current_map;
    DrawMapAndEntities(gamestate, map, dt);

    AnimateEntity(&gamestate->player);
    DrawSprite(gamestate->texture_atlas, gamestate->player.sprite, PLAYER_WIDTH, PLAYER_HEIGHT);
    EndMode2D();

    DrawCameraFade(gamestate);

    RenderHUD(gamestate);

    EndDrawing();
}
