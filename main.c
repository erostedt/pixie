#include "draw.h"
#include "floodfill.h"
#include "stdio.h"
#include "resize.h"

#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480

int main()
{
    Pixie_RGB_Image image = pixie_rgb_image_new(IMAGE_WIDTH, IMAGE_HEIGHT);
    pixie_draw_rectangle(&image, (Pixie_Rect){ (Pixie_Point){50, 50}, 120, 120}, RGB(255, 0, 0));
    pixie_draw_filled_circle(&image, (Pixie_Point){50, 70}, 40, RGB(0, 255, 0));
    
    pixie_draw_ellipse(&image, (Pixie_Point){200, 200}, 50, 80, RGB(0, 0, 255));
    pixie_draw_filled_ellipse(&image, (Pixie_Point){150, 150}, 100, 60, RGB(0, 0, 255));

    pixie_draw_triangle(&image, (Pixie_Point){100, 100}, (Pixie_Point){200, 200}, (Pixie_Point){100, 200}, RGB(255, 0, 0));

    pixie_floodfill(&image, (Pixie_Point){.x=102, .y=116}, RGB(255, 0, 0));

    pixie_draw_line(&image, (Pixie_Point){25, 100}, (Pixie_Point){100, 300}, RGB(255, 0, 0));
    pixie_draw_thick_line(&image, (Pixie_Point){40, 100}, (Pixie_Point){150, 300}, 5, RGB(255, 0, 0));
    pixie_rgb_image_save_as_ppm(&image, "image.ppm");

    Pixie_RGB_Image copy = pixie_rgb_image_copy(&image);
    pixie_resize_bilinear(&copy, (size_t)(IMAGE_WIDTH * 1.5), (size_t)(IMAGE_HEIGHT * 1.5));
    pixie_resize_nearest_neighbor(&image, (size_t)(IMAGE_WIDTH * 1.5), (size_t)(IMAGE_HEIGHT * 1.5));

    pixie_rgb_image_save_as_ppm(&copy, "image_bl.ppm");
    pixie_rgb_image_save_as_ppm(&image, "image_nn.ppm");
    
    return 0;
}