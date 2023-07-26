#include "stdlib.h"
#include "assert.h"
#include "stdio.h"
#include "memory.h"
#include "core.h"

#define PIXIE_LIST_GROWTH_FACTOR 2


typedef struct Pixie_Point_List
{
    Pixie_Point *points;
    size_t size;
    size_t capacity;
} Pixie_Point_List;


Pixie_Point_List pixie_point_list_new(size_t capacity)
{
    Pixie_Point *points = (Pixie_Point*)malloc(capacity * sizeof(Pixie_Point));
    assert(points != NULL);
    return (Pixie_Point_List){.points=points, .size=0, .capacity=capacity};
}


void pixie_point_list_free(Pixie_Point_List *list)
{
    free(list->points);
    list->capacity = 0;
    list->size = 0;
    list->points = NULL;
    list = NULL;
}


void pixie_point_list_resize(Pixie_Point_List *list, size_t new_cap)
{
    if (new_cap < list->size)
        printf("WARNING: Resized capacity smaller than original size, data might be lossed.");
    
    Pixie_Point *points = (Pixie_Point*)malloc(new_cap * sizeof(Pixie_Point));
    size_t n = (list->size < new_cap) ? list->size : new_cap;
    memcpy(points, list->points, n * sizeof(Pixie_Point));
    free(list->points);
    list->points = points;
    list->size = n;
    list->capacity = new_cap;
}


void pixie_point_list_append(Pixie_Point_List *list, Pixie_Point elem)
{
   while (list->size >= list->capacity)
   {
        pixie_point_list_resize(list, (size_t)((list->capacity + 1) * PIXIE_LIST_GROWTH_FACTOR));
   }

   list->points[list->size++] = elem;
}


Pixie_Point pixie_point_list_pop_unsafe(Pixie_Point_List *list)
{
    return list->points[--list->size];
}


void pixie_floodfill(Pixie_Image *image, Pixie_Point seed, uint32_t fill_value)
{
    assert((seed.x < image->width) && (seed.y < image->height));
    uint32_t *pixels = image->pixels;
    size_t stride = image->stride;
    uint32_t original_value = PIXEL_AT(pixels, seed.x, seed.y, stride);
    if (fill_value == original_value) return;

    Pixie_Point_List stack = pixie_point_list_new(20);
    pixie_point_list_append(&stack, seed);

    while (stack.size > 0)
    {
        Pixie_Point pt = pixie_point_list_pop_unsafe(&stack);
        uint32_t pixel = PIXEL_AT(pixels, pt.x, pt.y, stride);
        if (pixel != original_value)
            continue;

        PIXEL_AT(pixels, pt.x, pt.y, stride) = fill_value;            

        if (pt.x > 0)
            pixie_point_list_append(&stack, (Pixie_Point){.x=pt.x-1, .y=pt.y});
        
        if (pt.x + 1 < image->width)
            pixie_point_list_append(&stack, (Pixie_Point){.x=pt.x+1, .y=pt.y});
        
        if (pt.y > 0)
            pixie_point_list_append(&stack, (Pixie_Point){.x=pt.x, .y=pt.y-1});
        
        if (pt.y + 1 < image->height)
            pixie_point_list_append(&stack, (Pixie_Point){.x=pt.x, .y=pt.y+1});
    }

    pixie_point_list_free(&stack);
}