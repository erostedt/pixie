#include "draw.h"
#include "floodfill.h"
#include "stdio.h"
#include "resize.h"

#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480

int main()
{
    Pixie_Canvas canvas = pixie_canvas_new(IMAGE_WIDTH, IMAGE_HEIGHT);
    //pixie_draw_rectangle(&canvas, RECT(50, 50, 120, 120), PIXIE_RGBA(255, 0, 0, 255), 1);
    pixie_draw_triangle(&canvas, POINT(100, 100), POINT(200, 200), POINT(100, 200), PIXIE_RGBA(255, 0, 0, 255), 7);
    //pixie_draw_circle(&canvas, POINT(50, 70), 40, PIXIE_RGBA(0, 255, 0, 255), 3);
    /*
    pixie_draw_ellipse(&canvas, POINT(200, 200), 50, 80, PIXIE_RGBA(0, 0, 255, 255));
    pixie_draw_filled_ellipse(&canvas, POINT(150, 150), 100, 60, PIXIE_RGBA(0, 0, 255, 255));


    pixie_floodfill(&canvas, POINT(102, 116), PIXIE_RGBA(255, 0, 0, 255));

    pixie_draw_line(&canvas, POINT(25, 100), POINT(100, 300), PIXIE_RGBA(255, 0, 0, 255));
    pixie_draw_thick_line(&canvas, POINT(40, 100), POINT(150, 300), 5, PIXIE_RGBA(255, 0, 0, 255));
    */
    pixie_canvas_save_as_ppm(&canvas, "image.ppm");
    /*
    Pixie_Canvas copy = pixie_canvas_copy(&canvas);
    pixie_resize_bilinear(&copy, (size_t)(IMAGE_WIDTH * 1.5), (size_t)(IMAGE_HEIGHT * 1.5));
    pixie_resize_nearest_neighbor(&canvas, (size_t)(IMAGE_WIDTH * 1.5), (size_t)(IMAGE_HEIGHT * 1.5));

    pixie_canvas_save_as_ppm(&copy, "image_bl.ppm");
    pixie_canvas_save_as_ppm(&canvas, "image_nn.ppm");
    */    
    return 0;
}