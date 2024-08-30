#pragma once
#include "stdlib.h"
#include "stdint.h"
#include "rect.h"

typedef struct Pixie_Canvas
{
    size_t width;
    size_t height;
    size_t stride;
    uint32_t *pixels;
} Pixie_Canvas;

Pixie_Canvas pixie_canvas_new(size_t width, size_t height);
Pixie_Canvas pixie_canvas_copy(Pixie_Canvas *canvas);
Pixie_Canvas pixie_canvas_crop(Pixie_Canvas *canvas, Pixie_Rect region);
void pixie_canvas_free(Pixie_Canvas *canvas);
void pixie_canvas_fill(Pixie_Canvas *canvas, uint32_t color);
void pixie_canvas_save_as_ppm(Pixie_Canvas *canvas, const char *file_path);
