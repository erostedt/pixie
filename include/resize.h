#pragma once
#include "canvas.h"
#include "math.h"
#include "rgba.h"

void pixie_resize_bilinear(PixieCanvas *canvas, size_t target_width, size_t target_height)
{
    PixieCanvas resized_canvas = pixie_canvas_new(target_width, target_height);

    float xratio = (target_width > 1) ? ((float)canvas->width - 1) / (target_width - 1) : 0.0;
    float yratio = (target_height > 1) ? ((float)canvas->height - 1) / (target_height - 1) : 0.0;

    for (size_t r = 0; r < target_height; r++)
    {
        float y = yratio * r;
        float yl = floorf(y);
        float yw = y - yl;
        float one_minus_yw = 1.0f - yw;
        size_t ylow = yl;
        size_t yhigh = ceilf(y);

        for (size_t c = 0; c < target_width; c++)
        {
            float x = xratio * c;
            float xl = floorf(x);
            float xw = x - xl;
            float one_minus_xw = 1.0f - xw;
            size_t xlow = xl;
            size_t xhigh = ceilf(x);

            rgba32 tl = PIXEL_AT(canvas, xlow, ylow);
            rgba32 tr = PIXEL_AT(canvas, xhigh, ylow);
            rgba32 bl = PIXEL_AT(canvas, xlow, yhigh);
            rgba32 br = PIXEL_AT(canvas, xhigh, yhigh);

            float tlw = one_minus_xw * one_minus_yw;
            float trw = xw * one_minus_yw;
            float blw = one_minus_xw * yw;
            float brw = xw * yw;

            uint8_t red =
                fminf(255.0f, PIXIE_RED(tl) * tlw + PIXIE_RED(tr) * trw + PIXIE_RED(bl) * blw + PIXIE_RED(br) * brw);
            uint8_t green = fminf(255.0f, PIXIE_GREEN(tl) * tlw + PIXIE_GREEN(tr) * trw + PIXIE_GREEN(bl) * blw +
                                              PIXIE_GREEN(br) * brw);
            uint8_t blue = fminf(255.0f, PIXIE_BLUE(tl) * tlw + PIXIE_BLUE(tr) * trw + PIXIE_BLUE(bl) * blw +
                                             PIXIE_BLUE(br) * brw);

            uint8_t alpha = fminf(255.0f, PIXIE_ALPHA(tl) * tlw + PIXIE_ALPHA(tr) * trw + PIXIE_ALPHA(bl) * blw +
                                              PIXIE_ALPHA(br) * brw);

            PIXEL_AT(&resized_canvas, c, r) = PIXIE_RGBA(red, green, blue, alpha);
        }
    }
    pixie_canvas_swap(canvas, &resized_canvas);
    pixie_canvas_free(&resized_canvas);
}

static void pixie_resize_nearest_neighbor(PixieCanvas *canvas, size_t target_width, size_t target_height)
{
    PixieCanvas resized_canvas = pixie_canvas_new(target_width, target_height);
    float xratio = (target_width > 1) ? ((float)canvas->width - 1) / (target_width - 1) : 0.0;
    float yratio = (target_height > 1) ? ((float)canvas->height - 1) / (target_height - 1) : 0.0;

    for (size_t r = 0; r < target_height; r++)
    {
        size_t y = roundf(yratio * r);
        for (size_t c = 0; c < target_width; c++)
        {
            size_t x = roundf(xratio * c);
            PIXEL_AT(&resized_canvas, c, r) = PIXEL_AT(canvas, x, y);
        }
    }
    pixie_canvas_swap(canvas, &resized_canvas);
    pixie_canvas_free(&resized_canvas);
}
