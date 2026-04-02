#ifndef VEC2_H_
#define VEC2_H_

#include <SDL3/SDL_log.h>
#include <math.h>

typedef struct
{
    float x;
    float y;
} Vec2;

static inline Vec2 vec2(float x, float y) { return (Vec2){x, y}; }
static inline Vec2 vec2_zero() { return (Vec2){0.0f, 0.0f}; }
static inline Vec2 vec2_add(Vec2 a, Vec2 b) { return (Vec2){a.x + b.x, a.y + b.y}; }
static inline Vec2 vec2_scale(Vec2 a, float b) { return (Vec2){a.x * b, a.y * b}; }
static inline float vec2_len(Vec2 v) { return sqrtf(v.x * v.x + v.y * v.y); }
static inline Vec2 vec2_norm(Vec2 v)
{
    float len = vec2_len(v);
    return len != 0.0f ? vec2_scale(v, 1 / len) : vec2_zero();
}

#endif // VEC2_H_
