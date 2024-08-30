#pragma once
#include "stdlib.h"

typedef struct PixiePoint
{
    size_t x;
    size_t y;
} PixiePoint;

static inline PixiePoint POINT(size_t x, size_t y)
{
    return (PixiePoint){.x = x, .y = y};
}

static inline void swap_points(PixiePoint *p1, PixiePoint *p2)
{
    PixiePoint temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}
