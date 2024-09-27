#pragma once
#include <stddef.h>

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

PixiePoint POINT(size_t x, size_t y);
