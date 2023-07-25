#ifndef _PIXIE_DRAW_H
#define _PIXIE_DRAW_H
#include "core.h"


void pixie_draw_filled_rectangle(Pixie_Canvas *canvas, Pixie_Rect rect, uint32_t color);
void pixie_draw_rectangle(Pixie_Canvas *canvas, Pixie_Rect rect, uint32_t color);

void pixie_draw_filled_square(Pixie_Canvas *canvas, Pixie_Point top_left, size_t width, uint32_t color);
void pixie_draw_square(Pixie_Canvas *canvas, Pixie_Point top_left, size_t width, uint32_t color);

void pixie_draw_filled_circle(Pixie_Canvas *canvas, Pixie_Point center, size_t radius, uint32_t color);
void pixie_draw_circle(Pixie_Canvas *canvas, Pixie_Point center, size_t radius, uint32_t color);

void pixie_draw_filled_ellipse(Pixie_Canvas *canvas, Pixie_Point center, size_t a, size_t b, uint32_t color);
void pixie_draw_ellipse(Pixie_Canvas *canvas, Pixie_Point center, size_t a, size_t b, uint32_t color);

void pixie_draw_filled_triangle(Pixie_Canvas *canvas, Pixie_Point p1, Pixie_Point p2, Pixie_Point p3, uint32_t color);
void pixie_draw_triangle(Pixie_Canvas *canvas, Pixie_Point p1, Pixie_Point p2, Pixie_Point p3, uint32_t color);

void pixie_draw_line(Pixie_Canvas *canvas, Pixie_Point p1, Pixie_Point p2, uint32_t color);
void pixie_draw_thick_line(Pixie_Canvas *canvas, Pixie_Point p1, Pixie_Point p2, size_t thickness, uint32_t color);

void pixie_draw_vline(Pixie_Canvas *canvas, size_t x, size_t ymin, size_t ymax, uint32_t color);
void pixie_draw_hline(Pixie_Canvas *canvas, size_t y, size_t xmin, size_t xmax, uint32_t color);
void pixie_draw_thick_vline(Pixie_Canvas *canvas, size_t x , size_t ymin, size_t ymax, size_t thickness, uint32_t color);

static inline void pixie_draw_pixel_unsafe(Pixie_Canvas *canvas, size_t x, size_t y, uint32_t color) 
{ 
    PIXEL_AT(canvas->pixels, x, y, canvas->stride) = color;
}

static inline bool pixie_draw_pixel(Pixie_Canvas *canvas, size_t x, size_t y, uint32_t color) 
{
    if ((x < canvas->width) && (y < canvas->height))
    {
        pixie_draw_pixel_unsafe(canvas, x, y, color);
        return true;
    }
    return false;
}

static inline bool pixie_draw_pixel_int(uint32_t *pixels, int width, int height, int stride, int x, int y, uint32_t color)
{
    if ((x > -1) && (x < width) && (y < height) && (y > -1))
    {
        PIXEL_AT(pixels, x, y, stride) = color;
        return true;
    }
    return false;
}

#endif