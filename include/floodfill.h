#pragma once
#include "canvas.h"
#include "point.h"
#include "rgba.h"

void pixie_floodfill(Pixie_Canvas *canvas, Pixie_Point seed, rgba32 fill_color);
