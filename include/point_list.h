#pragma once
#include "assert.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "point.h"

#define PIXIE_LIST_GROWTH_FACTOR 2

typedef struct Pixie_Point_List
{
    Pixie_Point *points;
    size_t size;
    size_t capacity;
} Pixie_Point_List;


static Pixie_Point_List pixie_point_list_new(size_t capacity)
{
    Pixie_Point *points = (Pixie_Point*)malloc(capacity * sizeof(Pixie_Point));
    assert(points != NULL);
    return (Pixie_Point_List){.points=points, .size=0, .capacity=capacity};
}


static void pixie_point_list_free(Pixie_Point_List *list)
{
    free(list->points);
    list->capacity = 0;
    list->size = 0;
    list->points = NULL;
    list = NULL;
}


static void pixie_point_list_resize(Pixie_Point_List *list, size_t new_cap)
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


static void pixie_point_list_append(Pixie_Point_List *list, Pixie_Point elem)
{
    while (list->size >= list->capacity)
    {
        assert(PIXIE_LIST_GROWTH_FACTOR >= 1.0);
        pixie_point_list_resize(list, (size_t)((list->capacity + 1) * PIXIE_LIST_GROWTH_FACTOR));
    }

   list->points[list->size++] = elem;
}


static Pixie_Point pixie_point_list_pop_unsafe(Pixie_Point_List *list)
{
    return list->points[--list->size];
}
