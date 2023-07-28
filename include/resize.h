#ifndef _PIXIE_RESIZE_H
#define _PIXIE_RESIZE_H
#include "core.h"
#include "rgb.h"

void pixie_resize_bilinear(Pixie_RGB_Image *image, size_t target_width, size_t target_height);
void pixie_resize_nearest_neighbor(Pixie_RGB_Image *image, size_t target_width, size_t target_height);
#endif