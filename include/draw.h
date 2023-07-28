#ifndef _PIXIE_DRAW_H
#define _PIXIE_DRAW_H
#include "core.h"
#include "rgb.h"


void pixie_draw_filled_rectangle(Pixie_RGBImage *image, Pixie_Rect rect, rgb24 color);
void pixie_draw_rectangle(Pixie_RGBImage *image, Pixie_Rect rect, rgb24 color);

void pixie_draw_filled_square(Pixie_RGBImage *image, size_t x, size_t y, size_t width, rgb24 color);
void pixie_draw_square(Pixie_RGBImage *image, size_t x, size_t y, size_t width, rgb24 color);

void pixie_draw_filled_circle(Pixie_RGBImage *image, Pixie_Point center, size_t radius, rgb24 color);
void pixie_draw_circle(Pixie_RGBImage *image, Pixie_Point center, size_t radius, rgb24 color);

void pixie_draw_filled_ellipse(Pixie_RGBImage *image, Pixie_Point center, size_t a, size_t b, rgb24 color);
void pixie_draw_ellipse(Pixie_RGBImage *image, Pixie_Point center, size_t a, size_t b, rgb24 color);

void pixie_draw_filled_triangle(Pixie_RGBImage *image, Pixie_Point p1, Pixie_Point p2, Pixie_Point p3, rgb24 color);
void pixie_draw_triangle(Pixie_RGBImage *image, Pixie_Point p1, Pixie_Point p2, Pixie_Point p3, rgb24 color);

void pixie_draw_line(Pixie_RGBImage *image, Pixie_Point p1, Pixie_Point p2, rgb24 color);
void pixie_draw_thick_line(Pixie_RGBImage *image, Pixie_Point p1, Pixie_Point p2, size_t thickness, rgb24 color);

void pixie_draw_vline(Pixie_RGBImage *image, size_t x, size_t ymin, size_t ymax, rgb24 color);
void pixie_draw_hline(Pixie_RGBImage *image, size_t y, size_t xmin, size_t xmax, rgb24 color);
void pixie_draw_thick_vline(Pixie_RGBImage *image, size_t x , size_t ymin, size_t ymax, size_t thickness, rgb24 color);

static inline void pixie_draw_pixel_unsafe(Pixie_RGBImage *image, size_t x, size_t y, rgb24 color) 
{ 
    PIXEL_AT(image->pixels, x, y, image->stride) = color;
}

static inline bool pixie_draw_pixel(Pixie_RGBImage *image, size_t x, size_t y, rgb24 color) 
{
    if ((x < image->width) && (y < image->height))
    {
        pixie_draw_pixel_unsafe(image, x, y, color);
        return true;
    }
    return false;
}

static inline bool pixie_draw_pixel_int(rgb24 *pixels, int width, int height, int stride, int x, int y, rgb24 color)
{
    if ((x > -1) && (x < width) && (y < height) && (y > -1))
    {
        PIXEL_AT(pixels, x, y, stride) = color;
        return true;
    }
    return false;
}

#endif