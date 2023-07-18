#include "canvas.h"
#include "stdio.h"

#define CANVAS_WIDTH 640
#define CANVAS_HEIGHT 480

int main()
{
    Pixie_Canvas canvas = pixie_canvas_new(CANVAS_WIDTH, CANVAS_HEIGHT);
    pixie_draw_rectangle(&canvas, (Pixie_Rect){50, 50, 120, 120}, COLOR(255, 0, 0, 255));
    pixie_draw_circle(&canvas, 30, 70, 40, COLOR(0, 255, 0, 255));
    
    pixie_draw_ellipse(&canvas, 200, 200, 50, 80, COLOR(0, 0, 255, 255));
    pixie_draw_filled_ellipse(&canvas, 150, 150, 100, 60, COLOR(0, 0, 255, 255));

    pixie_draw_pixel(&canvas, 200, 200, COLOR(0,0,255, 255));

    pixie_draw_line(&canvas, 0, 0, 650, 300, COLOR(100, 255, 255, 255));
    pixie_canvas_save_as_ppm(&canvas, "canvas.ppm");
    return 0;
}