#include "point.h"

PixiePoint POINT(size_t x, size_t y)
{
    return (PixiePoint){.x = x, .y = y};
}
