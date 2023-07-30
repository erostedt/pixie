#ifndef _PIXIE_DRAW_H
#define _PIXIE_DRAW_H
#include "core.h"
#include "stdbool.h"

void pixie_draw_rectangle(Pixie_Canvas *canvas, Pixie_Rect rect, uint32_t color, size_t thickness);
void pixie_draw_square(Pixie_Canvas *canvas, size_t x, size_t y, size_t width, uint32_t color, size_t thickness);
void pixie_draw_circle(Pixie_Canvas *canvas, Pixie_Point center, size_t radius, uint32_t color, size_t thickness);
void pixie_draw_ellipse(Pixie_Canvas *canvas, Pixie_Point center, size_t a, size_t b, uint32_t color, size_t thickness);
void pixie_draw_triangle(Pixie_Canvas *canvas, Pixie_Point p1, Pixie_Point p2, Pixie_Point p3, uint32_t color, size_t thickness);
void pixie_draw_line(Pixie_Canvas *canvas, Pixie_Point p1, Pixie_Point p2, uint32_t color, size_t thickness);
void pixie_draw_vline(Pixie_Canvas *canvas, size_t x, size_t ymin, size_t ymax, uint32_t color, size_t thickness);
void pixie_draw_hline(Pixie_Canvas *canvas, size_t y, size_t xmin, size_t xmax, uint32_t color, size_t thickness);

#endif