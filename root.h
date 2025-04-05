// Thanks clangd.
#pragma once

#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#define INTERNAL 1

#define TARGET_FPS 60

#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 64

#define PRESSURE_PLATE_SIZE 64


#include "types.h"
#include "sprite.h"
#include "entity.h"
#include "tilemap.h"
#include "collision.h"
#include "game.h"

#include "sprite.c"
#include "entity.c"
#include "tilemap.c"
#include "collision.c"
#include "game.c"
