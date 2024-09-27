#pragma once
#include "canvas.h"

void pixie_resize_bilinear(PixieCanvas *canvas, size_t target_width, size_t target_height);
void pixie_resize_nearest_neighbor(PixieCanvas *canvas, size_t target_width, size_t target_height);
