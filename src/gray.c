#include "gray.h"
#include "stdlib.h"
#include "assert.h"
#include "stdio.h"
#include "math.h"
#include "memory.h"


Pixie_GrayImage pixie_gray_image_new(size_t width, size_t height)
{
    uint8_t *pixels = calloc(width * height, sizeof(uint8_t));
    assert(pixels != NULL);
    return (Pixie_GrayImage){ .width=width, .height=height, .stride=width, .pixels=pixels };
}


Pixie_GrayImage pixie_gray_image_crop(Pixie_GrayImage *image, Pixie_Rect region)
{
    assert(((region.x + region.w) < image->width) && ((region.y + region.h) < image->height));
    uint8_t *data = image->pixels + (region.y * image->stride + region.x);
    return (Pixie_GrayImage){ .width=region.w, .height=region.h, .stride=image->stride, .pixels=data };
}


void pixie_gray_image_free(Pixie_GrayImage *image)
{
    image->height = image->stride = image->width = 0;
    free(image->pixels);
    image = NULL;
}


void pixie_gray_image_fill(Pixie_GrayImage *image, uint8_t color)
{
    uint8_t *pixels = image->pixels;
    size_t width = image->width;
    size_t height = image->height;
    size_t stride = image->stride;
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
            PIXEL_AT(pixels, x, y, stride) = color;
    }
}

Pixie_GrayImage pixie_gray_image_copy(Pixie_GrayImage *image)
{
    uint8_t *pixels = (uint8_t*)malloc(image->width * image->height * sizeof(uint8_t));
    assert(pixels != NULL);
    memcpy(pixels, image->pixels, image->height * image->width * sizeof(uint8_t));
    return (Pixie_GrayImage){.width=image->width, .height=image->height, .stride=image->stride, .pixels=pixels};

}


void pixie_gray_image_save_as_ppm(Pixie_GrayImage *image, const char *file_path)
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
    
    uint8_t *pixels = image->pixels;


    fprintf(f, "P6\n%zu %zu 255\n", width, height);

    for (size_t y = 0; y < height; ++y) 
    {
        for (size_t x = 0; x < width; ++x) 
        {
            uint8_t pixel = PIXEL_AT(pixels, x, y, stride);
            uint8_t buf[3] = {pixel, pixel, pixel};
            
            fwrite(buf, sizeof(buf), 1, f);
        }
    }

    fclose(f);
}


