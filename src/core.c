#include "core.h"
#include "stdlib.h"
#include "assert.h"
#include "stdio.h"
#include "math.h"


Pixie_Rect pixie_rect_new(Pixie_Point top_left, size_t width, size_t height)
{
    return (Pixie_Rect){.top_left=top_left, .width=width, .height=height};
}


Pixie_Canvas pixie_canvas_new(size_t width, size_t height)
{
    uint32_t *pixels = calloc(width * height, sizeof(uint32_t));
    assert(pixels != NULL);
    return (Pixie_Canvas){ .width=width, .height=height, .stride=width, .pixels=pixels };
}


Pixie_Canvas pixie_subcanvas_new(Pixie_Canvas *canvas, Pixie_Rect region)
{
    assert(((region.top_left.x + region.width) < canvas->width) && ((region.top_left.y + region.height) < canvas->height));
    uint32_t *data = canvas->pixels + (region.top_left.y * canvas->stride + region.top_left.x);
    return (Pixie_Canvas) { .width=region.width, .height=region.height, .stride=canvas->stride, .pixels=data };
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
        for (size_t x = 0; x < width; x++)
            PIXEL_AT(pixels, x, y, stride) = color;
    }
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
        for (size_t x = 0; x < width; ++x) 
        {
            uint32_t pixel = PIXEL_AT(pixels, x, y, stride);
            uint8_t buf[3] = {(uint8_t)RED(pixel), (uint8_t)GREEN(pixel), (uint8_t)BLUE(pixel)};
            
            fwrite(buf, sizeof(buf), 1, f);
        }
    }

    fclose(f);
}


