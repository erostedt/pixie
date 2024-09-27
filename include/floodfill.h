#pragma once
#include "canvas.h"
#include "point.h"
#include "point_list.h"
#include "rgba.h"

void pixie_floodfill(PixieCanvas *canvas, PixiePoint seed, rgba32 fill_color);
