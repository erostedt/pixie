#pragma once
#include "canvas.h"
#include "point.h"
#include "point_list.h"
#include "rgba.h"

void pixie_floodfill(PixieCanvas *canvas, PixiePoint seed, rgba32 fill_color)
{
    assert((seed.x < canvas->width) && (seed.y < canvas->height));
    rgba32 original_color = PIXEL_AT(canvas, seed.x, seed.y);
    if (fill_color == original_color)
        return;

    size_t list_cap = canvas->width * canvas->height / 16;

    PixiePointList stack = pixie_point_list_new(list_cap);
    pixie_point_list_append(&stack, seed);

    while (stack.size > 0)
    {
        PixiePoint pt = pixie_point_list_pop_unsafe(&stack);
        rgba32 pixel = PIXEL_AT(canvas, pt.x, pt.y);
        if (pixel != original_color)
            continue;

        PIXEL_AT(canvas, pt.x, pt.y) = fill_color;;

        if (pt.x > 0)
            pixie_point_list_append(&stack, (PixiePoint){.x = pt.x - 1, .y = pt.y});

        if (pt.x + 1 < canvas->width)
            pixie_point_list_append(&stack, (PixiePoint){.x = pt.x + 1, .y = pt.y});

        if (pt.y > 0)
            pixie_point_list_append(&stack, (PixiePoint){.x = pt.x, .y = pt.y - 1});

        if (pt.y + 1 < canvas->height)
            pixie_point_list_append(&stack, (PixiePoint){.x = pt.x, .y = pt.y + 1});
    }

    pixie_point_list_free(&stack);
}
