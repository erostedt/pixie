#ifndef _PIXIE_CORE_H
#define _PIXIE_CORE_H

#include "stdlib.h"
#include "stdbool.h"

#define PIXEL_AT(pixels, x, y, stride) ((pixels)[(y) * (stride) + (x)])
#define RED_FROM_HEX(hex) (((hex) >> 24) & 0xFF)
#define GREEN_FROM_HEX(hex) (((hex) >> 16) & 0xFF)
#define BLUE_FROM_HEX(hex) (((hex) >> 8) & 0xFF)
#define ALPHA_FROM_HEX(hex) (((hex) >> 0) & 0xFF)
#define RGBA_TO_HEX(r, g, b, a) (((r) << 24) | ((g) << 16) | ((b) << 8) | (a))

typedef struct Pixie_Point
{
    size_t x, y;
} Pixie_Point;


typedef struct Pixie_Rect
{
    Pixie_Point top_left;
    size_t width;
    size_t height;
} Pixie_Rect;


typedef struct rgb24
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb24;


rgb24 RGB(uint8_t red, uint8_t green, uint8_t blue);
static inline bool rgb_equals(rgb24 pixel1, rgb24 pixel2)
{
    return (pixel1.r == pixel2.r) && (pixel1.g == pixel2.g) && (pixel1.b == pixel2.b);
}


typedef struct rgba32
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} rgba32;


typedef struct Pixie_RGB_Image
{
    size_t width;
    size_t height;
    size_t stride;
    rgb24 *pixels;
} Pixie_RGB_Image;

Pixie_RGB_Image pixie_rgb_image_new(size_t width, size_t height);
Pixie_RGB_Image pixie_rgb_image_copy(Pixie_RGB_Image *image);
Pixie_RGB_Image pixie_rgb_subimage_new(Pixie_RGB_Image *image, Pixie_Rect region);
void pixie_rgb_image_free(Pixie_RGB_Image *image);
void pixie_rgb_image_fill(Pixie_RGB_Image *image, rgb24 color);
void pixie_rgb_image_save_as_ppm(Pixie_RGB_Image *image, const char *file_path);

static inline void swap_points(Pixie_Point *p1, Pixie_Point *p2)
{
    Pixie_Point temp = *p1;
    *p1 = *p2;
    *p2 = temp; 
}
#endif
