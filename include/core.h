#ifndef _PIXIE_CORE_H
#define _PIXIE_CORE_H
#include "stdlib.h"


typedef struct Pixie_Point
{
    size_t x, y;
} Pixie_Point;

static inline Pixie_Point POINT(size_t x, size_t y)
{
    return (Pixie_Point){.x=x, .y=y};
}

static inline void swap_points(Pixie_Point *p1, Pixie_Point *p2)
{
    Pixie_Point temp = *p1;
    *p1 = *p2;
    *p2 = temp; 
}

typedef struct Pixie_Rect
{
    size_t x, y, w, h;
} Pixie_Rect;


static inline Pixie_Rect RECT(size_t x, size_t y, size_t w, size_t h)
{
    return (Pixie_Rect){.x=x, .y=y, .w=w, .h=h};
}

#endif