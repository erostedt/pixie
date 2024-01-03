
#include "draw.h"
#include "floodfill.h"
#include <stdio.h>

#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480

int main(int argc, char **argv)
{
    Pixie_Canvas canvas = pixie_canvas_new(IMAGE_WIDTH, IMAGE_HEIGHT);
    uint32_t color = PIXIE_RGBA(255, 0, 0, 255);
    pixie_draw_hline(&canvas, 100, 100, 400, color, 1);
    pixie_draw_hline(&canvas, 300, 100, 400, color, 1);
    pixie_draw_vline(&canvas, 100, 100, 300, color, 1);
    pixie_draw_vline(&canvas, 400, 100, 300, color, 1);
    pixie_canvas_save_as_ppm(&canvas, "floodfill_pre.ppm");
    pixie_floodfill(&canvas, POINT(200, 200), color);
    pixie_canvas_save_as_ppm(&canvas, "floodfill_post.ppm");
    return 0;
}
