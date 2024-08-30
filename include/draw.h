#pragma once
#include "core.h"
#include "point.h"

void pixie_draw_filled_rectangle(Pixie_Canvas *canvas, Pixie_Rect rect, uint32_t color);
void pixie_draw_filled_ellipse(Pixie_Canvas *canvas, Pixie_Point center, size_t a, size_t b, uint32_t color);
void pixie_draw_filled_triangle(Pixie_Canvas *canvas, Pixie_Point p1, Pixie_Point p2, Pixie_Point p3, uint32_t color);

void pixie_draw_hollow_rectangle(Pixie_Canvas *canvas, Pixie_Rect rect, uint32_t color);
void pixie_draw_hollow_ellipse(Pixie_Canvas *canvas, Pixie_Point center, size_t a, size_t b, uint32_t color);
void pixie_draw_hollow_triangle(Pixie_Canvas *canvas, Pixie_Point p1, Pixie_Point p2, Pixie_Point p3, uint32_t color);

void pixie_draw_line(Pixie_Canvas *canvas, Pixie_Point p1, Pixie_Point p2, uint32_t color, size_t thickness);
void pixie_draw_vline(Pixie_Canvas *canvas, size_t x, size_t ymin, size_t ymax, uint32_t color, size_t thickness);
void pixie_draw_hline(Pixie_Canvas *canvas, size_t y, size_t xmin, size_t xmax, uint32_t color, size_t thickness);
