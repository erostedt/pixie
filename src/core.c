#include "core.h"
#include "assert.h"
#include "math.h"
#include "memory.h"
#include "stdio.h"
#include "stdlib.h"

Pixie_Canvas pixie_canvas_new(size_t width, size_t height)
{
    assert((width > 1) && (height > 1));
    uint32_t *pixels = calloc(width * height, sizeof(uint32_t));
    assert(pixels != NULL);
    return (Pixie_Canvas){.width = width, .height = height, .stride = width, .pixels = pixels};
}

Pixie_Canvas pixie_canvas_crop(Pixie_Canvas *canvas, Pixie_Rect region)
{
    assert(((region.x + region.w) < canvas->width) && ((region.y + region.h) < canvas->height));
    uint32_t *data = canvas->pixels + (region.y * canvas->stride + region.x);
    return (Pixie_Canvas){.width = region.w, .height = region.h, .stride = canvas->stride, .pixels = data};
}

void pixie_canvas_free(Pixie_Canvas *canvas)
{
    canvas->height = canvas->stride = canvas->width = 0;
    free(canvas->pixels);
    canvas = NULL;
}

void pixie_canvas_fill(Pixie_Canvas *canvas, uint32_t color)
{
    uint32_t *pixels = canvas->pixels;
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

Pixie_Canvas pixie_canvas_copy(Pixie_Canvas *canvas)
{
    uint32_t *pixels = (uint32_t *)malloc(canvas->width * canvas->height * sizeof(uint32_t));
    assert(pixels != NULL);
    memcpy(pixels, canvas->pixels, canvas->height * canvas->width * sizeof(uint32_t));
    return (Pixie_Canvas){.width = canvas->width, .height = canvas->height, .stride = canvas->stride, .pixels = pixels};
}

void pixie_canvas_save_as_ppm(Pixie_Canvas *canvas, const char *file_path)
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

    uint32_t *pixels = canvas->pixels;

    fprintf(f, "P6\n%zu %zu 255\n", width, height);

    for (size_t y = 0; y < height; ++y)
    {
        size_t ys = y * stride;
        for (size_t x = 0; x < width; ++x)
        {
            uint32_t pixel = pixels[ys + x];
            uint8_t buf[3] = {PIXIE_RED(pixel), PIXIE_GREEN(pixel), PIXIE_BLUE(pixel)};

            fwrite(buf, sizeof(buf), 1, f);
        }
    }

    fclose(f);
}
