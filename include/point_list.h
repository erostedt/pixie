#pragma once
#include <stdlib.h>

#include "point.h"

typedef struct PixiePointList
{
    PixiePoint *points;
    size_t size;
    size_t capacity;
} PixiePointList;

PixiePointList pixie_point_list_new(size_t capacity);
void pixie_point_list_free(PixiePointList *list);
void pixie_point_list_resize(PixiePointList *list, size_t new_cap);
void pixie_point_list_append(PixiePointList *list, PixiePoint elem);
PixiePoint pixie_point_list_pop_unsafe(PixiePointList *list);
