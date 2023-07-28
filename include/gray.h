#ifndef _PIXIE_GRAY_H
#define _PIXIE_GRAY_H

#include "core.h"
#include "stdlib.h"
#include "stdbool.h"


typedef struct Pixie_GrayImage
{
    size_t width;
    size_t height;
    size_t stride;
    uint8_t *pixels;
} Pixie_GrayImage;

Pixie_GrayImage pixie_gray_image_new(size_t width, size_t height);
Pixie_GrayImage pixie_gray_image_copy(Pixie_GrayImage *image);
Pixie_GrayImage pixie_gray_image_crop(Pixie_GrayImage *image, Pixie_Rect region);
void pixie_gray_image_free(Pixie_GrayImage *image);
void pixie_gray_image_fill(Pixie_GrayImage *image, uint8_t color);
void pixie_gray_image_save_as_ppm(Pixie_GrayImage *image, const char *file_path);
#endif
