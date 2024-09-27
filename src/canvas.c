#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "canvas.h"

PixieCanvas pixie_canvas_new(size_t width, size_t height)
{
    assert((width > 0) && (height > 0));
    rgba32 *pixels = calloc(width * height, sizeof(rgba32));
    assert(pixels != NULL);
    return (PixieCanvas){.width = width, .height = height, .stride = width, .pixels = pixels};
}

PixieCanvas pixie_canvas_crop(PixieCanvas *canvas, PixieRect region)
{
    size_t right = pixie_rect_right(&region);
    size_t bottom = pixie_rect_bottom(&region);
    assert((region.w > 0) && (right < canvas->width) && (region.h > 0) && (bottom < canvas->height));
    rgba32 *data = &PIXEL_AT_POINT(canvas, region.top_left);
    return (PixieCanvas){.width = region.w, .height = region.h, .stride = canvas->stride, .pixels = data};
}

void pixie_canvas_free(PixieCanvas *canvas)
{
    canvas->height = canvas->stride = canvas->width = 0;
    free(canvas->pixels);
    canvas = NULL;
}

void pixie_canvas_fill(PixieCanvas *canvas, rgba32 color)
{
    for (size_t y = 0; y < canvas->height; y++)
    {
        for (size_t x = 0; x < canvas->width; x++)
        {
            PIXEL_AT(canvas, x, y) = color;
        }
    }
}

PixieCanvas pixie_canvas_copy(PixieCanvas *canvas)
{
    rgba32 *pixels = malloc(canvas->width * canvas->height * sizeof(rgba32));
    assert(pixels != NULL);
    memcpy(pixels, canvas->pixels, canvas->height * canvas->width * sizeof(rgba32));
    return (PixieCanvas){.width = canvas->width, .height = canvas->height, .stride = canvas->stride, .pixels = pixels};
}

void pixie_canvas_swap(PixieCanvas *canvas1, PixieCanvas *canvas2)
{
    PixieCanvas temp_canvas = {0};
    memcpy(&temp_canvas, canvas1, sizeof(PixieCanvas));
    memcpy(canvas1, canvas2, sizeof(PixieCanvas));
    memcpy(canvas2, &temp_canvas, sizeof(PixieCanvas));
}

void pixie_canvas_save_as_ppm(PixieCanvas *canvas, const char *file_path)
{
    FILE *f = fopen(file_path, "w");
    if (f == NULL)
    {
        fprintf(stderr, "ERROR: could not open file %s: %m\n", file_path);
        exit(1);
    }
    const size_t width = canvas->width;
    const size_t height = canvas->height;

    fprintf(f, "P3\n%zu %zu\n255\n", width, height);

    for (size_t y = 0; y < height; ++y)
    {
        for (size_t x = 0; x < width - 1; ++x)
        {
            rgba32 pixel = PIXEL_AT(canvas, x, y);
            fprintf(f, "%d %d %d ", PIXIE_RED(pixel), PIXIE_GREEN(pixel), PIXIE_BLUE(pixel));
        }
        rgba32 pixel = PIXEL_AT(canvas, width - 1, y);
        fprintf(f, "%d %d %d\n", PIXIE_RED(pixel), PIXIE_GREEN(pixel), PIXIE_BLUE(pixel));
    }

    fclose(f);
}
