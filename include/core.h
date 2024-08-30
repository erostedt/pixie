#pragma once
#include "stdint.h"
#include "stdlib.h"

#define PIXEL_AT(canvas, x, y) ((canvas).pixels[(y) * ((canvas).stride) + (x)])
#define PIXIE_RED(pixel) (((pixel) >> 24) & 0xFF)
#define PIXIE_GREEN(pixel) (((pixel) >> 16) & 0xFF)
#define PIXIE_BLUE(pixel) (((pixel) >> 8) & 0xFF)
#define PIXIE_ALPHA(pixel) (((pixel) >> 0) & 0xFF)
#define PIXIE_RGBA(r, g, b, a) (((r) << 24) | ((g) << 16) | ((b) << 8) | (a))


typedef struct Pixie_Rect {
  size_t x, y, w, h;
} Pixie_Rect;

static inline Pixie_Rect RECT(size_t x, size_t y, size_t w, size_t h) {
  return (Pixie_Rect){.x = x, .y = y, .w = w, .h = h};
}

typedef struct Pixie_Canvas {
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
