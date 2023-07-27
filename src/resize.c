#include "resize.h"
#include "assert.h"
#include "math.h"

void pixie_resize_bilinear(Pixie_Image *image, size_t target_width, size_t target_height)
{
    uint32_t *new_pixels = (uint32_t*)malloc(target_height * target_width * sizeof(uint32_t));
    assert(new_pixels != NULL);

    size_t new_stride = target_width;

    uint32_t *pixels = image->pixels;
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

            uint32_t a = PIXEL_AT(pixels, xlow, ylow, stride);
            uint32_t b = PIXEL_AT(pixels, xhigh, ylow, stride);
            uint32_t c = PIXEL_AT(pixels, xlow, yhigh, stride);
            uint32_t d = PIXEL_AT(pixels, xhigh, yhigh, stride);

            uint8_t red = fminf(255.0f, 
                RED(a) * one_minus_xw * one_minus_yw + 
                RED(b) * xw * one_minus_yw + 
                RED(c) * yw * one_minus_xw + 
                RED(d) * xw * yw
            );

            uint8_t green = fminf(255.0f, 
                GREEN(a) * one_minus_xw * one_minus_yw + 
                GREEN(b) * xw * one_minus_yw + 
                GREEN(c) * yw * one_minus_xw + 
                GREEN(d) * xw * yw
            );

            uint8_t blue = fminf(255.0f, 
                BLUE(a) * one_minus_xw * one_minus_yw + 
                BLUE(b) * xw * one_minus_yw + 
                BLUE(c) * yw * one_minus_xw + 
                BLUE(d) * xw * yw
            );
 
            uint8_t alpha = fminf(255.0f, 
                ALPHA(a) * one_minus_xw * one_minus_yw + 
                ALPHA(b) * xw * one_minus_yw + 
                ALPHA(c) * yw * one_minus_xw + 
                ALPHA(d) * xw * yw
            );    
            
            PIXEL_AT(new_pixels, j, i, new_stride) = COLOR(red, green, blue, alpha);
        }
    }

    free(image->pixels);
    image->pixels = new_pixels;
    image->stride = new_stride;
    image->width = target_width;
    image->height = target_height;
}


void pixie_resize_nearest_neighbor(Pixie_Image *image, size_t target_width, size_t target_height)
{
    uint32_t *new_pixels = (uint32_t*)malloc(target_height * target_width * sizeof(uint32_t));
    assert(new_pixels != NULL);

    size_t new_stride = target_width;

    uint32_t *pixels = image->pixels;
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