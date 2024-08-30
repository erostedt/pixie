#pragma once
#include "stdlib.h"

typedef struct Pixie_Point
{
    size_t x;
    size_t y;
} Pixie_Point;

static inline Pixie_Point POINT(size_t x, size_t y)
{
    return (Pixie_Point){.x = x, .y = y};
}

static inline void swap_points(Pixie_Point *p1, Pixie_Point *p2)
{
    Pixie_Point temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}
