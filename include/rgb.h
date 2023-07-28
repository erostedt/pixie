#ifndef _PIXIE_RGB_H
#define _PIXIE_RGB_H

#include "core.h"
#include "stdlib.h"
#include "stdbool.h"

#define RED_FROM_HEX(hex) (((hex) >> 24) & 0xFF)
#define GREEN_FROM_HEX(hex) (((hex) >> 16) & 0xFF)
#define BLUE_FROM_HEX(hex) (((hex) >> 8) & 0xFF)


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


typedef struct Pixie_RGB_Image
{
    size_t width;
    size_t height;
    size_t stride;
    rgb24 *pixels;
} Pixie_RGBImage;

Pixie_RGBImage pixie_rgb_image_new(size_t width, size_t height);
Pixie_RGBImage pixie_rgb_image_copy(Pixie_RGBImage *image);
Pixie_RGBImage pixie_rgb_image_crop(Pixie_RGBImage *image, Pixie_Rect region);
void pixie_rgb_image_free(Pixie_RGBImage *image);
void pixie_rgb_image_fill(Pixie_RGBImage *image, rgb24 color);
void pixie_rgb_image_save_as_ppm(Pixie_RGBImage *image, const char *file_path);
#endif
