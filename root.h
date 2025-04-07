// Thanks clangd.
#pragma once

#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#define INTERNAL 0
#define TARGET_FPS 60
#define CAMERA_ZOOM 1.5f

#include "types.h"
#include "maths.h"
#include "HUD.h"
#include "sprite.h"
#include "entity.h"
#include "tilemap.h"
#include "collision.h"
#include "game.h"

global b32 global_running;
global b32 global_show_tootip;

#include "sprite.c"
#include "entity.c"
#include "tilemap.c"
#include "collision.c"
#include "HUD.c"
#include "game.c"

