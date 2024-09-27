#pragma once
#include <stdint.h>
#include <stdlib.h>

#include "rect.h"
#include "rgba.h"

typedef struct PixieCanvas
{
    size_t width;
    size_t height;
    size_t stride;
    rgba32 *pixels;
} PixieCanvas;

PixieCanvas pixie_canvas_new(size_t width, size_t height);
PixieCanvas pixie_canvas_crop(PixieCanvas *canvas, PixieRect region);
void pixie_canvas_free(PixieCanvas *canvas);
void pixie_canvas_fill(PixieCanvas *canvas, rgba32 color);
PixieCanvas pixie_canvas_copy(PixieCanvas *canvas);
void pixie_canvas_swap(PixieCanvas *canvas1, PixieCanvas *canvas2);
void pixie_canvas_save_as_ppm(PixieCanvas *canvas, const char *file_path);
