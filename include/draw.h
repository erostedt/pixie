#pragma once
#include <assert.h>
#include <math.h>

#include "canvas.h"
#include "point.h"
#include "rect.h"
#include "rgba.h"

void pixie_blend_colors_avg(rgba32 *rgba1, rgba32 rgba2);
void pixie_draw_filled_rectangle(PixieCanvas *canvas, PixieRect rect, rgba32 color);
void pixie_draw_hollow_rectangle(PixieCanvas *canvas, PixieRect rect, rgba32 color);
void pixie_draw_filled_ellipse(PixieCanvas *canvas, PixiePoint center, size_t a, size_t b, rgba32 color);
void pixie_draw_hollow_ellipse(PixieCanvas *canvas, PixiePoint center, size_t a, size_t b, rgba32 color);
void pixie_draw_filled_triangle(PixieCanvas *canvas, PixiePoint p1, PixiePoint p2, PixiePoint p3, rgba32 color);
void pixie_draw_hollow_triangle(PixieCanvas *canvas, PixiePoint p1, PixiePoint p2, PixiePoint p3, rgba32 color);
void pixie_draw_hline(PixieCanvas *canvas, size_t y, size_t xmin, size_t xmax, rgba32 color, size_t thickness);
void pixie_draw_vline(PixieCanvas *canvas, size_t x, size_t ymin, size_t ymax, rgba32 color, size_t thickness);
void pixie_draw_line(PixieCanvas *canvas, PixiePoint p1, PixiePoint p2, rgba32 color, size_t thickness);
