#ifndef _PIXIE_COLOR_H
#define _PIXIE_COLOR_H
#include "stdlib.h"
#include "rgb.h"
#include "gray.h"

static inline uint8_t rgb2gray(rgb24 rgb)
{
    float r = rgb.r, g = rgb.g, b = rgb.b;
    float gray = 0.3f * r + 0.59f * g + 0.11f * b;
    if (gray > 255.0f) gray = 255.0f;
    return (uint8_t)gray;
}

static inline rgb24 gray2rgb(uint8_t gray)
{
    return (rgb24){.r=gray, .g=gray, .b=gray};
}



Pixie_GrayImage pixie_rgb2gray(Pixie_RGBImage *image);
Pixie_RGBImage pixie_gray2rgb(Pixie_GrayImage *image);

#endif