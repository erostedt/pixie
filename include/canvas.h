#pragma once
#include "assert.h"
#include "rect.h"
#include "rgba.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

typedef struct Pixie_Canvas
{
    size_t width;
    size_t height;
    size_t stride;
    rgba32 *pixels;
} Pixie_Canvas;

static Pixie_Canvas pixie_canvas_new(size_t width, size_t height)
{
    assert((width > 1) && (height > 1));
    rgba32 *pixels = calloc(width * height, sizeof(rgba32));
    assert(pixels != NULL);
    return (Pixie_Canvas){.width = width, .height = height, .stride = width, .pixels = pixels};
}

static Pixie_Canvas pixie_canvas_crop(Pixie_Canvas *canvas, Pixie_Rect region)
{
    assert(((region.x + region.w) < canvas->width) && ((region.y + region.h) < canvas->height));
    rgba32 *data = canvas->pixels + (region.y * canvas->stride + region.x);
    return (Pixie_Canvas){.width = region.w, .height = region.h, .stride = canvas->stride, .pixels = data};
}

static void pixie_canvas_free(Pixie_Canvas *canvas)
{
    canvas->height = canvas->stride = canvas->width = 0;
    free(canvas->pixels);
    canvas = NULL;
}

static void pixie_canvas_fill(Pixie_Canvas *canvas, rgba32 color)
{
    rgba32 *pixels = canvas->pixels;
    size_t width = canvas->width;
    size_t height = canvas->height;
    size_t stride = canvas->stride;
    for (size_t y = 0; y < height; y++)
    {
        size_t ys = y * stride;
        for (size_t x = 0; x < width; x++)
            pixels[ys + x] = color;
    }
}

static Pixie_Canvas pixie_canvas_copy(Pixie_Canvas *canvas)
{
    rgba32 *pixels = malloc(canvas->width * canvas->height * sizeof(rgba32));
    assert(pixels != NULL);
    memcpy(pixels, canvas->pixels, canvas->height * canvas->width * sizeof(rgba32));
    return (Pixie_Canvas){.width = canvas->width, .height = canvas->height, .stride = canvas->stride, .pixels = pixels};
}

static void pixie_canvas_save_as_ppm(Pixie_Canvas *canvas, const char *file_path)
{
    FILE *f = fopen(file_path, "wb");
    if (f == NULL)
    {
        fprintf(stderr, "ERROR: could not open file %s: %m\n", file_path);
        exit(1);
    }
    size_t stride = canvas->stride;
    size_t width = canvas->width;
    size_t height = canvas->height;

    rgba32 *pixels = canvas->pixels;

    fprintf(f, "P6\n%zu %zu 255\n", width, height);

    for (size_t y = 0; y < height; ++y)
    {
        size_t ys = y * stride;
        for (size_t x = 0; x < width; ++x)
        {
            rgba32 pixel = pixels[ys + x];
            uint8_t buf[3] = {PIXIE_RED(pixel), PIXIE_GREEN(pixel), PIXIE_BLUE(pixel)};

            fwrite(buf, sizeof(buf), 1, f);
        }
    }

    fclose(f);
}
