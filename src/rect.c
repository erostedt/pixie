#include <stdint.h>

#include "point.h"
#include "rect.h"

PixieRect RECT(size_t x, size_t y, size_t w, size_t h)
{
    return (PixieRect){.top_left = POINT(x, y), .w = w, .h = h};
}

size_t pixie_rect_left(const PixieRect *rect)
{
    return rect->top_left.x;
}

size_t pixie_rect_right(const PixieRect *rect)
{
    return rect->top_left.x + rect->w;
}

size_t pixie_rect_top(const PixieRect *rect)
{
    return rect->top_left.y;
}

size_t pixie_rect_bottom(const PixieRect *rect)
{
    return rect->top_left.y + rect->h;
}
