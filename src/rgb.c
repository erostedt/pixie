#include "rgb.h"
#include "stdlib.h"
#include "assert.h"
#include "stdio.h"
#include "math.h"
#include "memory.h"


Pixie_RGB_Image pixie_rgb_image_new(size_t width, size_t height)
{
    rgb24 *pixels = calloc(width * height, sizeof(rgb24));
    assert(pixels != NULL);
    return (Pixie_RGB_Image){ .width=width, .height=height, .stride=width, .pixels=pixels };
}


Pixie_RGB_Image pixie_rgb_subimage_new(Pixie_RGB_Image *image, Pixie_Rect region)
{
    assert(((region.x + region.w) < image->width) && ((region.y + region.h) < image->height));
    rgb24 *data = image->pixels + (region.y * image->stride + region.x);
    return (Pixie_RGB_Image){ .width=region.w, .height=region.h, .stride=image->stride, .pixels=data };
}


void pixie_rgb_image_free(Pixie_RGB_Image *image)
{
    image->height = image->stride = image->width = 0;
    free(image->pixels);
    image = NULL;
}


void pixie_rgb_image_fill(Pixie_RGB_Image *image, rgb24 color)
{
    rgb24 *pixels = image->pixels;
    size_t width = image->width;
    size_t height = image->height;
    size_t stride = image->stride;
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
            PIXEL_AT(pixels, x, y, stride) = color;
    }
}

Pixie_RGB_Image pixie_rgb_image_copy(Pixie_RGB_Image *image)
{
    rgb24 *pixels = (rgb24*)malloc(image->width * image->height * sizeof(rgb24));
    assert(pixels != NULL);
    memcpy(pixels, image->pixels, image->height * image->width * sizeof(rgb24));
    return (Pixie_RGB_Image){.width=image->width, .height=image->height, .stride=image->stride, .pixels=pixels};

}

rgb24 RGB(uint8_t red, uint8_t green, uint8_t blue)
{
    return (rgb24){.r=red, .g=green, .b=blue};
}


void pixie_rgb_image_save_as_ppm(Pixie_RGB_Image *image, const char *file_path)
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
    
    rgb24 *pixels = image->pixels;


    fprintf(f, "P6\n%zu %zu 255\n", width, height);

    for (size_t y = 0; y < height; ++y) 
    {
        for (size_t x = 0; x < width; ++x) 
        {
            rgb24 pixel = PIXEL_AT(pixels, x, y, stride);
            uint8_t buf[3] = {pixel.r, pixel.g, pixel.b};
            
            fwrite(buf, sizeof(buf), 1, f);
        }
    }

    fclose(f);
}


