#pragma once
#include "root.h"

function CollisionResult
CheckCollision(Tilemap *map, Entity *player, v2 *velocity, Axis axis)
{
    Rectangle player_rect =
    {
        player->pos.x,
        player->pos.y + PLAYER_WIDTH,
        PLAYER_WIDTH,
        PLAYER_HEIGHT*0.5f
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
                entity_rect = (Rectangle){ other->pos.x, other->pos.y, PRESSURE_PLATE_SIZE-10, PRESSURE_PLATE_SIZE-10 };
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
HandleOverlappingCollision(GameState *gamestate, Tilemap *map, Entity *e)
{
    if(e)
    {
        switch(e->type)
        {
            case PressurePlate:
            {
                if(gamestate->plate_sequence.sequence_len > 0)
                {
                    if(!e->activated)
                    {
                        e->activated = true;
                        e->collision_flag = CollisionFlag_None;

                        // Check sequence
                        PlateSequence *seq = &gamestate->plate_sequence;

                        if(seq->sequence_failed || seq->sequence_completed)
                            return;

                        if(e->plate_color == seq->color_sequence[seq->current_index])
                        {
                            seq->current_index++;

                            if(seq->current_index >= seq->sequence_len)
                            {
                                // Success
                                seq->sequence_completed = true;
                                PlaySound(gamestate->door1);
                            }
                            else
                            {
                                // Correct step
                                PlaySound(gamestate->blip1);
                            }
                        }
                        else
                        {
                            // Failure
                            if(seq->current_index > 0)
                                PlaySound(gamestate->door2);

                            ResetPlates(map);
                            seq->sequence_failed = false;
                            seq->current_index = 0;
                        }
                    }
                }
                else
                {
                    // Only clone can trigger blue plates..
                    if(e->plate_color == PlateColor_Blue)
                    {
                        return;
                    }

                    if(!e->activated)
                    {
                        e->activated = true;
                        e->collision_flag = CollisionFlag_None;
                        PlaySound(gamestate->blip1);
                    }
                }
            } break;

            case Door:
            {
                e->collision_flag = CollisionFlag_None;
                e->type = None;
                IncrementLevel(gamestate);

            } break;

            default: break;
        }
    }
}
