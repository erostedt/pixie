#pragma once
#include "stdint.h"

typedef uint32_t rgba32; 
#define PIXEL_AT(canvas, x, y) ((canvas)->pixels[(y) * ((canvas)->stride) + (x)])
#define PIXIE_RED(pixel) (((pixel) >> 24) & 0xFF)
#define PIXIE_GREEN(pixel) (((pixel) >> 16) & 0xFF)
#define PIXIE_BLUE(pixel) (((pixel) >> 8) & 0xFF)
#define PIXIE_ALPHA(pixel) (((pixel) >> 0) & 0xFF)
#define PIXIE_RGBA(r, g, b, a) (((r) << 24) | ((g) << 16) | ((b) << 8) | (a))



