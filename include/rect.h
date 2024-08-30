#pragma once
#include "point.h"
#include "stdint.h"
#include "stdlib.h"

typedef struct PixieRect
{
    PixiePoint top_left;
    size_t w, h;
} PixieRect;

static inline PixieRect RECT(size_t x, size_t y, size_t w, size_t h)
{
    return (PixieRect){.top_left = POINT(x, y), .w = w, .h = h};
}

static inline size_t pixie_rect_left(const PixieRect *rect)
{
    return rect->top_left.x;
}

static inline size_t pixie_rect_right(const PixieRect *rect)
{
    return rect->top_left.x + rect->w;
}

static inline size_t pixie_rect_top(const PixieRect *rect)
{
    return rect->top_left.y;
}

static inline size_t pixie_rect_bottom(const PixieRect *rect)
{
    return rect->top_left.y + rect->h;
}
