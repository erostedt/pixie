#ifndef _PIXIE_CORE_H
#define _PIXIE_CORE_H

#include "stdlib.h"
#include "stdbool.h"

#define PIXEL_AT(pixels, x, y, stride) ((pixels)[(y) * (stride) + (x)])
#define RED(pixel) (((pixel) >> 24) & 0xFF)
#define GREEN(pixel) (((pixel) >> 16) & 0xFF)
#define BLUE(pixel) (((pixel) >> 8) & 0xFF)
#define ALPHA(pixel) (((pixel) >> 0) & 0xFF)
#define COLOR(r, g, b, a) (((r) << 24) | ((g) << 16) | ((b) << 8) | (a))

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

Pixie_Rect pixie_rect_new(Pixie_Point top_left, size_t width, size_t height);


typedef struct Pixie_Image
{
    size_t width;
    size_t height;
    size_t stride;
    uint32_t *pixels;
} Pixie_Image;

Pixie_Image pixie_image_new(size_t width, size_t height);
Pixie_Image pixie_subimage_new(Pixie_Image *image, Pixie_Rect region);
void pixie_image_free(Pixie_Image *image);
void pixie_image_fill(Pixie_Image *image, uint32_t color);
void pixie_image_save_as_ppm(Pixie_Image *image, const char *file_path);

static inline void swap_points(Pixie_Point *p1, Pixie_Point *p2)
{
    Pixie_Point temp = *p1;
    *p1 = *p2;
    *p2 = temp; 
}
#endif
