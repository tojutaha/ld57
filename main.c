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
typedef int32_t b32;
#define function static

#define MAP_HEIGHT 12
#define MAP_WIDTH 12
#define TILE_WIDTH 64
#define TILE_HEIGHT 64

#define MAX_ENTITIES 100
#define ENTITY_MAX_VELOCITY 200
#define PLAYER_SIZE 32
#define PRESSURE_PLATE_SIZE 64

typedef struct
{
    u32 level;
} GameState;

typedef enum
{
    Axis_X,
    Axis_Y,
} Axis;

typedef enum
{
    CollisionFlag_None    = 0,
    CollisionFlag_Overlap = 1 << 0,
    CollisionFlag_Block   = 1 << 1,
} CollisionFlag;

typedef enum
{
    Player,
    Door,
    Obstacle,
    PressurePlate,
} EntityType;

function const char *
PrettifyEntityType(EntityType type)
{
    switch(type)
    {
    case Player: return "Player";
    case Door: return "Door";
    case Obstacle: return "Obstacle";
    case PressurePlate: return "PressurePlate";
    default: return "Unknown";
    }
}

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
    CollisionFlag collision_flag;

    b32 pressure_plate_active;

} Entity;

typedef struct
{
    u32 pressure_plate_count;
    b32 door_open;

    u32 entity_count;
    Entity entities[MAX_ENTITIES];
} Tilemap;

function void
InitTilemap(Tilemap *map)
{
    map->door_open = false;

    for(u32 y = 0; y < MAP_HEIGHT; ++y)
    {
        for(u32 x = 0; x < MAP_WIDTH; ++x)
        {
            f32 tile_x = x * TILE_WIDTH;
            f32 tile_y = y * TILE_HEIGHT;

            if(x == MAP_WIDTH/2 && y == 0)
            {
                Entity door = { (v2){tile_x, tile_y}, Vector2Zero(), 0, Door, Left, CollisionFlag_Block };
                map->entities[map->entity_count++] = door;
                continue;
            }

            if(x == 0 || x == MAP_WIDTH-1 || y == 0 || y == MAP_HEIGHT-1)
            {
                Entity wall = { (v2){tile_x, tile_y}, Vector2Zero(), 0, Obstacle, Left, CollisionFlag_Block };
                map->entities[map->entity_count++] = wall;
            }
        }
    }
}

function void
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

            case Door:
            {
                if(map->pressure_plate_count <= 0)
                {
                    map->door_open = true;
                    e->collision_flag = CollisionFlag_None;
                }

                Color color = map->door_open ? GOLD : DARKGREEN;

                DrawRectangleV(e->pos, (v2){ TILE_WIDTH, TILE_HEIGHT }, color);
            } break;

            case PressurePlate:
            {
                Color color = e->pressure_plate_active ? RED : BLUE;
                DrawRectangleV(e->pos, (v2){ PRESSURE_PLATE_SIZE, PRESSURE_PLATE_SIZE }, color);
            }

            default: break;
        }
    }
}

typedef struct
{
    Entity *collided_entity;
    v2 normal;
    b32 blocking;
    b32 overlapping;
} CollisionResult;

function CollisionResult
CheckCollision(Tilemap *map, Entity *player, v2 *velocity, Axis axis)
{
    Rectangle player_rect =
    {
        player->pos.x,
        player->pos.y,
        PLAYER_SIZE,
        PLAYER_SIZE
    };

    CollisionResult result = {0};

    for(u32 entity_index = 0; entity_index < map->entity_count; ++entity_index)
    {
        Entity *other = map->entities + entity_index;

        Rectangle entity_rect = {0};
        switch(other->type)
        {
            case PressurePlate:
            {
                entity_rect = (Rectangle){ other->pos.x, other->pos.y, PRESSURE_PLATE_SIZE, PRESSURE_PLATE_SIZE };
            } break;

            default:
            {
                entity_rect = (Rectangle){ other->pos.x, other->pos.y, TILE_WIDTH, TILE_HEIGHT };
            } break;
        }

        if(CheckCollisionRecs(player_rect, entity_rect))
        {
            if(other->collision_flag & CollisionFlag_Block)
            {
                v2 player_center =
                {
                    player_rect.x + player_rect.width * 0.5f,
                    player_rect.y + player_rect.height * 0.5f
                };

                v2 other_center =
                {
                    entity_rect.x + entity_rect.width * 0.5f,
                    entity_rect.y + entity_rect.height * 0.5f,
                };

                f32 dx = player_center.x - other_center.x;
                f32 dy = player_center.y - other_center.y;

                f32 half_width = (player_rect.width + entity_rect.width) * 0.5f;
                f32 half_height = (player_rect.height + entity_rect.height) * 0.5f;

                f32 overlap_x = half_width - fabsf(dx);
                f32 overlap_y = half_height - fabsf(dy);

                if(overlap_x < overlap_y)
                {
                    result.normal.x = (dx > 0) ? 1.0f : -1.0f;
                }
                else
                {
                    result.normal.y = (dy > 0) ? 1.0f : -1.0f;
                }

                if(axis == Axis_X && result.normal.x != 0)
                {
                    player->pos.x += result.normal.x * overlap_x;

                    if(velocity)
                    {
                        f32 dot = Vector2DotProduct(*velocity, result.normal);
                        if(dot < 0)
                        {
                            v2 correction = Vector2Scale(result.normal, dot);
                            *velocity = Vector2Subtract(*velocity, correction);
                        }
                    }

                    player_rect.x = player->pos.x;
                }
                else if(axis == Axis_Y && result.normal.y != 0)
                {
                    player->pos.y += result.normal.y * overlap_y;

                    if(velocity)
                    {
                        f32 dot = Vector2DotProduct(*velocity, result.normal);
                        if(dot < 0)
                        {
                            v2 correction = Vector2Scale(result.normal, dot);
                            *velocity = Vector2Subtract(*velocity, correction);
                        }
                    }

                    player_rect.y = player->pos.y;
                }

                result.collided_entity = other;
                result.blocking = true;
                return result;

            }
            else if(other->collision_flag & CollisionFlag_Overlap)
            {
                result.collided_entity = other;
                result.overlapping = true;
                return result;
            }
        }
    }

    return result;
}

function void
HandleOverlappingCollision(Tilemap *map, Entity *e)
{
    if(e)
    {
        // printf("Overlapping collision with entity %s\n", PrettifyEntityType(e->type));

        switch(e->type)
        {
            case PressurePlate:
            {
                if(!e->pressure_plate_active)
                {
                    e->pressure_plate_active = true;
                    map->pressure_plate_count--;
                    // e->collision_flag = CollisionFlag_None;
                }
            } break;

            case Door:
            {
                printf("Collided with door\n");
            } break;

            default: break;
        }
    }
}

function void
UpdatePlayer(Tilemap *map, Entity *player, Camera2D *camera, f32 dt)
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
        HandleOverlappingCollision(map, collision_result_x.collided_entity);
    }

    if(collision_result_y.overlapping)
    {
        Entity *e = collision_result_y.collided_entity;
        HandleOverlappingCollision(map, collision_result_y.collided_entity);
    }

    camera->target = player->pos;
}

function void
AddPressurePlate(Tilemap *map, s32 x, s32 y)
{
    Entity e =
    {
        // .pos = (v2){ x * TILE_WIDTH + PRESSURE_PLATE_SIZE * 0.5f, y * TILE_HEIGHT + PRESSURE_PLATE_SIZE * 0.5f},
        .pos = (v2){ x * TILE_WIDTH + PRESSURE_PLATE_SIZE, y * TILE_HEIGHT + PRESSURE_PLATE_SIZE },
        .vel = Vector2Zero(),
        .speed = 0,
        .type = PressurePlate,
        .collision_flag = CollisionFlag_Overlap,
        .pressure_plate_active = false,
    };

    map->entities[map->entity_count++] = e;
    map->pressure_plate_count++;
}

int main(void)
{
    s32 window_width = 800;
    s32 window_height = 600;
    SetConfigFlags(FLAG_VSYNC_HINT|FLAG_MSAA_4X_HINT);
    InitWindow(window_width, window_height, "LD57");
    SetTargetFPS(60);

    v2 start_pos = { window_width / 2.0f, window_height / 2.0f };
    Entity player = { start_pos, Vector2Zero(), 2000, Player, Left, CollisionFlag_Overlap };

    Camera2D camera = {0};
    camera.offset = start_pos;
    camera.zoom = 1.0f;
    camera.target = player.pos;

    Tilemap map = {0};
    InitTilemap(&map);

    AddPressurePlate(&map, 1, 1);
    AddPressurePlate(&map, 2, 2);
    AddPressurePlate(&map, 3, 3);

    GameState gamestate;
    gamestate.level = 1;

    while(!WindowShouldClose())
    {
        f32 dt = GetFrameTime();
        dt = Clamp(dt, dt, 1.0f/30.0f);

        UpdatePlayer(&map, &player, &camera, dt);

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);

        DrawMapAndEntities(&map);

        DrawRectangleV(player.pos, (v2){ 32, 32 }, BLUE);

        EndMode2D();

        DrawText(TextFormat("LEVEL: %d", gamestate.level), 20, 20, 20, GREEN);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
