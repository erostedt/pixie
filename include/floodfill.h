#pragma once
#include "canvas.h"
#include "point.h"
#include "rgba.h"

void pixie_floodfill(PixieCanvas *canvas, PixiePoint seed, RGBA32 fill_color);
