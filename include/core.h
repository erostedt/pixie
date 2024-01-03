#ifndef _PIXIE_CORE_H
#define _PIXIE_CORE_H
#include "stdint.h"
#include "stdlib.h"

#define PIXEL_AT(canvas, x, y) ((canvas).pixels[(y) * ((canvas).stride) + (x)])
#define PIXIE_RED(pixel) (((pixel) >> 24) & 0xFF)
#define PIXIE_GREEN(pixel) (((pixel) >> 16) & 0xFF)
#define PIXIE_BLUE(pixel) (((pixel) >> 8) & 0xFF)
#define PIXIE_ALPHA(pixel) (((pixel) >> 0) & 0xFF)
#define PIXIE_RGBA(r, g, b, a) (((r) << 24) | ((g) << 16) | ((b) << 8) | (a))

typedef struct Pixie_Point {
  size_t x, y;
} Pixie_Point;

static inline Pixie_Point POINT(size_t x, size_t y) {
  return (Pixie_Point){.x = x, .y = y};
}

static inline void swap_points(Pixie_Point *p1, Pixie_Point *p2) {
  Pixie_Point temp = *p1;
  *p1 = *p2;
  *p2 = temp;
}

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

#endif
