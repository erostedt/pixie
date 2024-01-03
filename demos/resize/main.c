
#include "draw.h"
#include "resize.h"

#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480

int main(int argc, char **argv)
{
    Pixie_Canvas canvas = pixie_canvas_new(IMAGE_WIDTH, IMAGE_HEIGHT);
    pixie_draw_filled_rectangle(&canvas, RECT(50, 50, 120, 120), PIXIE_RGBA(255, 0, 0, 255));
    pixie_draw_filled_ellipse(&canvas, POINT(200, 100), 45, 60, PIXIE_RGBA(0, 255, 0, 255));
    pixie_draw_filled_triangle(&canvas, POINT(100, 300), POINT(300, 200), POINT(300, 100), PIXIE_RGBA(0, 0, 255, 255));

    pixie_draw_hollow_rectangle(&canvas, RECT(350, 350, 120, 120), PIXIE_RGBA(255, 0, 0, 255));
    pixie_draw_hollow_ellipse(&canvas, POINT(500, 400), 45, 60, PIXIE_RGBA(0, 255, 0, 255));
    pixie_draw_hollow_triangle(&canvas, POINT(300, 300), POINT(300, 200), POINT(200, 100), PIXIE_RGBA(0, 0, 255, 255));

    pixie_canvas_save_as_ppm(&canvas, "resize_pre.ppm");
    Pixie_Canvas copy = pixie_canvas_copy(&canvas);
    pixie_resize_bilinear(&copy, (size_t)(IMAGE_WIDTH * 1.5), (size_t)(IMAGE_HEIGHT * 1.5));
    pixie_resize_nearest_neighbor(&canvas, (size_t)(IMAGE_WIDTH * 1.5), (size_t)(IMAGE_HEIGHT * 1.5));

    pixie_canvas_save_as_ppm(&copy, "resize_bl.ppm");
    pixie_canvas_save_as_ppm(&canvas, "resize_nn.ppm");
    return 0;
}
