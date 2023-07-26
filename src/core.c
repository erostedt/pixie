#include "core.h"
#include "stdlib.h"
#include "assert.h"
#include "stdio.h"
#include "math.h"


Pixie_Rect pixie_rect_new(Pixie_Point top_left, size_t width, size_t height)
{
    return (Pixie_Rect){.top_left=top_left, .width=width, .height=height};
}


Pixie_Image pixie_image_new(size_t width, size_t height)
{
    uint32_t *pixels = calloc(width * height, sizeof(uint32_t));
    assert(pixels != NULL);
    return (Pixie_Image){ .width=width, .height=height, .stride=width, .pixels=pixels };
}


Pixie_Image pixie_subimage_new(Pixie_Image *image, Pixie_Rect region)
{
    assert(((region.top_left.x + region.width) < image->width) && ((region.top_left.y + region.height) < image->height));
    uint32_t *data = image->pixels + (region.top_left.y * image->stride + region.top_left.x);
    return (Pixie_Image) { .width=region.width, .height=region.height, .stride=image->stride, .pixels=data };
}


void pixie_image_free(Pixie_Image *image)
{
    image->height = image->stride = image->width = 0;
    free(image->pixels);
    image = NULL;
}


void pixie_image_fill(Pixie_Image *image, uint32_t color)
{
    uint32_t *pixels = image->pixels;
    size_t width = image->width;
    size_t height = image->height;
    size_t stride = image->stride;
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
            PIXEL_AT(pixels, x, y, stride) = color;
    }
}


void pixie_image_save_as_ppm(Pixie_Image *image, const char *file_path)
{
    FILE *f = fopen(file_path, "wb");
    if (f == NULL) 
    {
        fprintf(stderr, "ERROR: could not open file %s: %m\n", file_path);
        exit(1);
    }
    size_t stride = image->stride;
    size_t width = image->width;
    size_t height = image->height;
    
    uint32_t *pixels = image->pixels;


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


