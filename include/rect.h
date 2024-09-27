#pragma once
#include "point.h"
#include "stdint.h"
#include "stdlib.h"

typedef struct PixieRect
{
    PixiePoint top_left;
    size_t w, h;
} PixieRect;

PixieRect RECT(size_t x, size_t y, size_t w, size_t h);
size_t pixie_rect_left(const PixieRect *rect);
size_t pixie_rect_right(const PixieRect *rect);
size_t pixie_rect_top(const PixieRect *rect);
size_t pixie_rect_bottom(const PixieRect *rect);
