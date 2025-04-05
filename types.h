#pragma once
#include "root.h"

typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint32_t u16;
typedef uint8_t u8;
typedef float f32;
typedef double f64;
typedef Vector2 v2;
typedef int32_t b32;

#define function static
#define global static
#define local_persist static

#define enum8(type) u8
#define enum16(type) u16
#define enum32(type) u32
#define enum64(type) u64

#if INTERNAL
#define Assert(Expr) if (!(Expr)) { __builtin_trap(); }
#else
#define Assert(Expr)
#endif
