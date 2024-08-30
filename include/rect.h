#pragma once
#include "stdint.h"
#include "stdlib.h"

typedef struct Pixie_Rect
{
    size_t x, y;
    size_t w, h;
} Pixie_Rect;

static inline Pixie_Rect RECT(size_t x, size_t y, size_t w, size_t h)
{
    return (Pixie_Rect){.x = x, .y = y, .w = w, .h = h};
}
