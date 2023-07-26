#include "draw.h"
#include "floodfill.h"
#include "stdio.h"

#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480

int main()
{
    Pixie_Image image = pixie_image_new(IMAGE_WIDTH, IMAGE_HEIGHT);
    pixie_draw_rectangle(&image, (Pixie_Rect){ (Pixie_Point){50, 50}, 120, 120}, COLOR(255, 0, 0, 255));
    pixie_draw_filled_circle(&image, (Pixie_Point){50, 70}, 40, COLOR(0, 255, 0, 255));
    
    pixie_draw_ellipse(&image, (Pixie_Point){200, 200}, 50, 80, COLOR(0, 0, 255, 255));
    pixie_draw_filled_ellipse(&image, (Pixie_Point){150, 150}, 100, 60, COLOR(0, 0, 255, 255));

    pixie_draw_triangle(&image, (Pixie_Point){100, 100}, (Pixie_Point){200, 200}, (Pixie_Point){100, 200}, COLOR(255, 0, 0, 255));

    pixie_floodfill(&image, (Pixie_Point){.x=102, .y=116}, COLOR(255, 0, 0, 255));

    pixie_draw_line(&image, (Pixie_Point){25, 100}, (Pixie_Point){100, 300}, COLOR(255, 0, 0, 255));
    pixie_draw_thick_line(&image, (Pixie_Point){40, 100}, (Pixie_Point){150, 300}, 5, COLOR(255, 0, 0, 255));
    pixie_image_save_as_ppm(&image, "image.ppm");
    return 0;
}