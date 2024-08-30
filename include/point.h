#pragma once
#include "stdlib.h"

typedef struct PixiePoint
{
    size_t x;
    size_t y;
} PixiePoint;

typedef struct PixiePointf
{
    float x;
    float y;
} PixiePointf;

static inline PixiePoint POINT(size_t x, size_t y)
{
    return (PixiePoint){.x = x, .y = y};
}
