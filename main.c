#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

typedef int32_t s32;
typedef uint32_t u32;
typedef float f32;
typedef double f64;
typedef Vector2 v2;

#define MAP_HEIGHT 10
#define MAP_WIDTH 10
#define TILE_WIDTH 64
#define TILE_HEIGHT 64

#define MAX_ENTITIES 100
#define ENTITY_MAX_VELOCITY 1000
#define ENTITY_SIZE 32

typedef enum
{
    Player,
    Obstacle,
    Monster,
} EntityType;

typedef enum
{
    Left,
    Right,
    Up,
    Down
} Direction;

typedef struct
{
    v2 pos;
    v2 vel;
    f32 speed;
    EntityType type;
    Direction dir;
} Entity;

typedef struct
{
    u32 entity_count;
    Entity entities[MAX_ENTITIES];
} Tilemap;

static void
InitTilemap(Tilemap *map)
{
    for(u32 y = 0; y < MAP_HEIGHT; ++y)
    {
        for(u32 x = 0; x < MAP_WIDTH; ++x)
        {
            f32 tile_x = x * TILE_WIDTH;
            f32 tile_y = y * TILE_HEIGHT;

            if(x == 0 || x == MAP_WIDTH-1 || y == 0 || y == MAP_HEIGHT-1)
            {
                Entity wall = { (v2){tile_x, tile_y}, Vector2Zero(), 0, Obstacle, Left };
                map->entities[map->entity_count++] = wall;
            }
        }
    }
}

static void
DrawMapAndEntities(Tilemap *map)
{
    for(u32 y = 0; y < MAP_HEIGHT; ++y)
    {
        for(u32 x = 0; x < MAP_WIDTH; ++x)
        {
            f32 tile_x = x * TILE_WIDTH;
            f32 tile_y = y * TILE_HEIGHT;

            DrawRectangle(tile_x, tile_y, TILE_WIDTH, TILE_HEIGHT, GREEN);
            DrawRectangleLines(tile_x, tile_y, TILE_WIDTH, TILE_HEIGHT, BLACK);
        }
    }

    for(u32 entity_index = 0; entity_index < map->entity_count; ++entity_index)
    {
        Entity *e = map->entities + entity_index;
        switch(e->type)
        {
            case Obstacle:
            {
                DrawRectangleV(e->pos, (v2){ TILE_WIDTH, TILE_HEIGHT }, DARKGREEN);
            } break;

            case Monster:
            {
                DrawRectangleV(e->pos, (v2){ ENTITY_SIZE, ENTITY_SIZE }, RED);
            }

            default: break;
        }
    }
}

static void
UpdatePlayer(Entity *player, Camera2D *camera, f32 dt)
{
    v2 movement_vector = {0};

    if(IsKeyDown(KEY_A)) movement_vector.x -= 1;
    if(IsKeyDown(KEY_D)) movement_vector.x += 1;
    if(IsKeyDown(KEY_W)) movement_vector.y -= 1;
    if(IsKeyDown(KEY_S)) movement_vector.y += 1;

    f32 len = Vector2LengthSqr(movement_vector);
    if(len > 0)
    {
        movement_vector.x *= 1.0f / sqrtf(len);
        movement_vector.y *= 1.0f / sqrtf(len);

        v2 direction_normalize = Vector2Normalize(movement_vector);

        if(direction_normalize.x < 0) player->dir = Left;
        if(direction_normalize.x > 0) player->dir = Right;
        if(direction_normalize.y < 0) player->dir = Up;
        if(direction_normalize.y > 0) player->dir = Down;
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

    player->pos.x += player_delta.x;
    player->pos.y += player_delta.y;
    camera->target = player->pos;
}

int main(void)
{
    s32 window_width = 800;
    s32 window_height = 600;
    SetConfigFlags(FLAG_VSYNC_HINT|FLAG_MSAA_4X_HINT);
    InitWindow(window_width, window_height, "LD57");
    SetTargetFPS(60);

    v2 start_pos = { window_width / 2.0f, window_height / 2.0f };
    Entity player = { start_pos, Vector2Zero(), 2000, Player, Left };

    Camera2D camera = {0};
    camera.offset = start_pos;
    camera.zoom = 1.0f;
    camera.target = player.pos;

    Tilemap map = {0};
    InitTilemap(&map);

    Entity monster = { (v2){ TILE_WIDTH*1.25f, TILE_HEIGHT*1.25f }, Vector2Zero(), 1800, Monster, Left };
    map.entities[map.entity_count++] = monster;

    while(!WindowShouldClose())
    {
        f32 dt = GetFrameTime();

        UpdatePlayer(&player, &camera, dt);

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);

        DrawMapAndEntities(&map);

        DrawRectangleV(player.pos, (v2){ 32, 32 }, BLUE);

        EndMode2D();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
