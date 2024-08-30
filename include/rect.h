#pragma once
#include "stdint.h"
#include "stdlib.h"

typedef struct PixieRect
{
    size_t x, y;
    size_t w, h;
} PixieRect;

static inline PixieRect RECT(size_t x, size_t y, size_t w, size_t h)
{
    return (PixieRect){.x = x, .y = y, .w = w, .h = h};
}
