#ifndef _PIXIE_CANVAS_H
#define _PIXIE_CANVAS_H

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


typedef struct Pixie_Canvas
{
    size_t width;
    size_t height;
    size_t stride;
    uint32_t *pixels;
} Pixie_Canvas;

Pixie_Canvas pixie_canvas_new(size_t width, size_t height);
Pixie_Canvas pixie_subcanvas_new(Pixie_Canvas *canvas, Pixie_Rect region);
void pixie_canvas_free(Pixie_Canvas *canvas);
void pixie_canvas_fill(Pixie_Canvas *canvas, uint32_t color);
void pixie_canvas_save_as_ppm(Pixie_Canvas *canvas, const char *file_path);

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

static inline void swap_points(Pixie_Point *p1, Pixie_Point *p2)
{
    Pixie_Point temp = *p1;
    *p1 = *p2;
    *p2 = temp; 
}
#endif
