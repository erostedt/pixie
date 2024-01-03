#include "resize.h"
#include "assert.h"
#include "core.h"
#include "math.h"

void pixie_resize_bilinear(Pixie_Canvas *canvas, size_t target_width, size_t target_height)
{
    uint32_t *new_pixels = (uint32_t *)malloc(target_height * target_width * sizeof(uint32_t));
    assert(new_pixels != NULL);

    size_t new_stride = target_width;

    uint32_t *pixels = canvas->pixels;
    size_t stride = canvas->stride;

    float xratio = (target_width > 1) ? ((float)canvas->width - 1) / (target_width - 1) : 0.0;
    float yratio = (target_height > 1) ? ((float)canvas->height - 1) / (target_height - 1) : 0.0;

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

            uint32_t a = pixels[ylow * stride + xlow];
            uint32_t b = pixels[ylow * stride + xhigh];
            uint32_t c = pixels[yhigh * stride + xlow];
            uint32_t d = pixels[yhigh * stride + xhigh];

            uint8_t red = fminf(255.0f, PIXIE_RED(a) * one_minus_xw * one_minus_yw + PIXIE_RED(b) * xw * one_minus_yw +
                                            PIXIE_RED(c) * yw * one_minus_xw + PIXIE_RED(d) * xw * yw);

            uint8_t green =
                fminf(255.0f, PIXIE_GREEN(a) * one_minus_xw * one_minus_yw + PIXIE_GREEN(b) * xw * one_minus_yw +
                                  PIXIE_GREEN(c) * yw * one_minus_xw + PIXIE_GREEN(d) * xw * yw);

            uint8_t blue =
                fminf(255.0f, PIXIE_BLUE(a) * one_minus_xw * one_minus_yw + PIXIE_BLUE(b) * xw * one_minus_yw +
                                  PIXIE_BLUE(c) * yw * one_minus_xw + PIXIE_BLUE(d) * xw * yw);

            uint8_t alpha =
                fminf(255.0f, PIXIE_ALPHA(a) * one_minus_xw * one_minus_yw + PIXIE_ALPHA(b) * xw * one_minus_yw +
                                  PIXIE_ALPHA(c) * yw * one_minus_xw + PIXIE_ALPHA(d) * xw * yw);

            new_pixels[i * new_stride + j] = PIXIE_RGBA(red, green, blue, alpha);
        }
    }

    free(canvas->pixels);
    canvas->pixels = new_pixels;
    canvas->stride = new_stride;
    canvas->width = target_width;
    canvas->height = target_height;
}

void pixie_resize_nearest_neighbor(Pixie_Canvas *canvas, size_t target_width, size_t target_height)
{
    uint32_t *new_pixels = (uint32_t *)malloc(target_height * target_width * sizeof(uint32_t));
    assert(new_pixels != NULL);

    size_t new_stride = target_width;

    uint32_t *pixels = canvas->pixels;
    size_t stride = canvas->stride;

    float xratio = (target_width > 1) ? ((float)canvas->width - 1) / (target_width - 1) : 0.0;
    float yratio = (target_height > 1) ? ((float)canvas->height - 1) / (target_height - 1) : 0.0;

    for (size_t i = 0; i < target_height; i++)
    {
        size_t y = roundf(yratio * i);
        size_t ys = y * stride;
        size_t is = i * new_stride;
        for (size_t j = 0; j < target_width; j++)
        {
            size_t x = roundf(xratio * j);
            new_pixels[is + j] = pixels[ys + x];
        }
    }

    free(canvas->pixels);
    canvas->pixels = new_pixels;
    canvas->stride = new_stride;
    canvas->width = target_width;
    canvas->height = target_height;
}
