#pragma once
#include "canvas.h"
#include "point.h"
#include "point_list.h"

void pixie_floodfill(Pixie_Canvas *canvas, Pixie_Point seed, uint32_t fill_color)
{
    assert((seed.x < canvas->width) && (seed.y < canvas->height));
    uint32_t *pixels = canvas->pixels;
    size_t stride = canvas->stride;
    uint32_t original_color = pixels[seed.y * stride + seed.x];
    if (fill_color == original_color) return;

    size_t list_cap = canvas->width * canvas->height / 16;

    Pixie_Point_List stack = pixie_point_list_new(list_cap);
    pixie_point_list_append(&stack, seed);

    while (stack.size > 0)
    {
        Pixie_Point pt = pixie_point_list_pop_unsafe(&stack);
        uint32_t pixel = pixels[pt.y * stride + pt.x];
        if (pixel != original_color)
            continue;

        pixels[pt.y * stride + pt.x] = fill_color;            

        if (pt.x > 0)
            pixie_point_list_append(&stack, (Pixie_Point){.x=pt.x-1, .y=pt.y});
        
        if (pt.x + 1 < canvas->width)
            pixie_point_list_append(&stack, (Pixie_Point){.x=pt.x+1, .y=pt.y});
        
        if (pt.y > 0)
            pixie_point_list_append(&stack, (Pixie_Point){.x=pt.x, .y=pt.y-1});
        
        if (pt.y + 1 < canvas->height)
            pixie_point_list_append(&stack, (Pixie_Point){.x=pt.x, .y=pt.y+1});
    }

    pixie_point_list_free(&stack);
}
