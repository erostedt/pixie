#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "point.h"
#include "point_list.h"

#define PIXIE_LIST_GROWTH_FACTOR 2

PixiePointList pixie_point_list_new(size_t capacity)
{
    PixiePoint *points = malloc(capacity * sizeof(PixiePoint));
    assert(points != NULL);
    return (PixiePointList){.points = points, .size = 0, .capacity = capacity};
}

void pixie_point_list_free(PixiePointList *list)
{
    free(list->points);
    list->capacity = 0;
    list->size = 0;
    list->points = NULL;
    list = NULL;
}

void pixie_point_list_resize(PixiePointList *list, size_t new_cap)
{
    if (new_cap < list->size)
        printf("WARNING: Resized capacity smaller than original size, data might be lossed.");

    PixiePoint *points = malloc(new_cap * sizeof(PixiePoint));
    size_t n = (list->size < new_cap) ? list->size : new_cap;
    memcpy(points, list->points, n * sizeof(PixiePoint));
    free(list->points);
    list->points = points;
    list->size = n;
    list->capacity = new_cap;
}

void pixie_point_list_append(PixiePointList *list, PixiePoint elem)
{
    while (list->size >= list->capacity)
    {
        assert(PIXIE_LIST_GROWTH_FACTOR >= 1.0);
        pixie_point_list_resize(list, (size_t)((list->capacity + 1) * PIXIE_LIST_GROWTH_FACTOR));
    }

    list->points[list->size++] = elem;
}

PixiePoint pixie_point_list_pop_unsafe(PixiePointList *list)
{
    return list->points[--list->size];
}
