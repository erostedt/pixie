#ifndef _PIXIE_RESIZE_H
#define _PIXIE_RESIZE_H
#include "core.h"

void pixie_resize_bilinear(Pixie_Image *image, size_t target_width, size_t target_height);
void pixie_resize_nearest_neighbor(Pixie_Image *image, size_t target_width, size_t target_height);
#endif