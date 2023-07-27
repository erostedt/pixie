#include "resize.h"
#include "assert.h"
#include "math.h"

void pixie_resize_bilinear(Pixie_RGB_Image *image, size_t target_width, size_t target_height)
{
    rgb24 *new_pixels = (rgb24*)malloc(target_height * target_width * sizeof(rgb24));
    assert(new_pixels != NULL);

    size_t new_stride = target_width;

    rgb24 *pixels = image->pixels;
    size_t stride = image->stride;

    float xratio = (target_width > 1) ? ((float)image->width - 1) / (target_width - 1) : 0.0;
    float yratio = (target_height > 1) ? ((float)image->height - 1) / (target_height - 1) : 0.0;

    for (size_t i = 0; i < target_height; i++)
    {
        float y = yratio * i;
        float yl = floorf(y);
        float yw = y - yl;
        float one_minus_yw = 1.0f - yw;
        size_t ylow = yl;
        size_t yhigh = ceilf(y);


        for (size_t j = 0; j < target_width; j++)
        {
            float x = xratio * j;
            float xl = floorf(x);
            float xw = x - xl;
            float one_minus_xw = 1.0f - xw;
            size_t xlow = xl;
            size_t xhigh = ceilf(x);

            rgb24 a = PIXEL_AT(pixels, xlow, ylow, stride);
            rgb24 b = PIXEL_AT(pixels, xhigh, ylow, stride);
            rgb24 c = PIXEL_AT(pixels, xlow, yhigh, stride);
            rgb24 d = PIXEL_AT(pixels, xhigh, yhigh, stride);

            uint8_t red = fminf(255.0f, 
                a.r * one_minus_xw * one_minus_yw + 
                b.r * xw * one_minus_yw + 
                c.r * yw * one_minus_xw + 
                d.r * xw * yw
            );

            uint8_t green = fminf(255.0f, 
                a.g * one_minus_xw * one_minus_yw + 
                b.g * xw * one_minus_yw + 
                c.g * yw * one_minus_xw + 
                d.g * xw * yw
            );

            uint8_t blue = fminf(255.0f, 
                a.b * one_minus_xw * one_minus_yw + 
                b.b * xw * one_minus_yw + 
                c.b * yw * one_minus_xw + 
                d.b * xw * yw
            );
 
            PIXEL_AT(new_pixels, j, i, new_stride) = (rgb24){.r=red, .g=green, .b=blue};
        }
    }

    free(image->pixels);
    image->pixels = new_pixels;
    image->stride = new_stride;
    image->width = target_width;
    image->height = target_height;
}


void pixie_resize_nearest_neighbor(Pixie_RGB_Image *image, size_t target_width, size_t target_height)
{
    rgb24 *new_pixels = (rgb24*)malloc(target_height * target_width * sizeof(rgb24));
    assert(new_pixels != NULL);

    size_t new_stride = target_width;

    rgb24 *pixels = image->pixels;
    size_t stride = image->stride;

    float xratio = (target_width > 1) ? ((float)image->width - 1) / (target_width - 1) : 0.0;
    float yratio = (target_height > 1) ? ((float)image->height - 1) / (target_height - 1) : 0.0;

    for (size_t i = 0; i < target_height; i++)
    {
        size_t y = roundf(yratio * i);
        for (size_t j = 0; j < target_width; j++)
        {
            size_t x = roundf(xratio * j);
            PIXEL_AT(new_pixels, j, i, new_stride) = PIXEL_AT(pixels, x, y, stride);
        }
    }

    free(image->pixels);
    image->pixels = new_pixels;
    image->stride = new_stride;
    image->width = target_width;
    image->height = target_height;
}