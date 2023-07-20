#include "canvas.h"
#include "stdio.h"

#define CANVAS_WIDTH 640
#define CANVAS_HEIGHT 480

int main()
{
    Pixie_Canvas canvas = pixie_canvas_new(CANVAS_WIDTH, CANVAS_HEIGHT);
    pixie_draw_rectangle(&canvas, (Pixie_Rect){ (Pixie_Point){50, 50}, 120, 120}, COLOR(255, 0, 0, 255));
    pixie_draw_filled_circle(&canvas, (Pixie_Point){50, 70}, 40, COLOR(0, 255, 0, 255));
    
    pixie_draw_ellipse(&canvas, (Pixie_Point){200, 200}, 50, 80, COLOR(0, 0, 255, 255));
    pixie_draw_filled_ellipse(&canvas, (Pixie_Point){150, 150}, 100, 60, COLOR(0, 0, 255, 255));

    pixie_draw_triangle(&canvas, (Pixie_Point){100, 100}, (Pixie_Point){200, 200}, (Pixie_Point){100, 200}, COLOR(255, 0, 0, 255));

    pixie_draw_line(&canvas, (Pixie_Point){25, 100}, (Pixie_Point){100, 300}, COLOR(255, 0, 0, 255));
    pixie_draw_thick_line(&canvas, (Pixie_Point){40, 100}, (Pixie_Point){150, 300}, 5, COLOR(255, 0, 0, 255));
    pixie_canvas_save_as_ppm(&canvas, "canvas.ppm");
    return 0;
}