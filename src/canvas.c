#include "canvas.h"
#include "stdlib.h"
#include "assert.h"
#include "stdio.h"


Pixie_Rect pixie_rect_new(int x, int y, size_t width, size_t height)
{
    return (Pixie_Rect){.x=x, .y=y, .width=width, .height=height};
}


Pixie_Canvas pixie_canvas_new(size_t width, size_t height)
{
    uint32_t *pixels = calloc(width * height, sizeof(uint32_t));
    assert(pixels != NULL);
    return (Pixie_Canvas){ .offset=0, .width=width, .height=height, .stride=width, .pixels=pixels };
}


void pixie_canvas_fill(size_t width, size_t height, uint32_t color)
{
}


void pixie_canvas_save_as_ppm(Pixie_Canvas *canvas, const char *file_path)
{
    FILE *f = fopen(file_path, "wb");
    if (f == NULL) 
    {
        fprintf(stderr, "ERROR: could not open file %s: %m\n", file_path);
        exit(1);
    }
    size_t stride = canvas->stride;
    size_t offset = canvas->offset;
    size_t width = canvas->width;
    size_t height = canvas->height;
    
    uint32_t *pixels = canvas->pixels;


    fprintf(f, "P6\n%zu %zu 255\n", width, height);

    for (size_t y = 0; y < height; ++y) 
    {
        for (size_t x = 0; x < width; ++x) 
        {
            uint32_t pixel = PIXEL_AT(pixels, x, y, stride, offset);
            uint8_t buf[3] = {(uint8_t)RED(pixel), (uint8_t)GREEN(pixel), (uint8_t)BLUE(pixel)};
            
            fwrite(buf, sizeof(buf), 1, f);
        }
    }

    fclose(f);
}

void pixie_draw_rectangle(Pixie_Canvas *canvas, Pixie_Rect rect, uint32_t color)
{
    size_t max_x = (canvas->width > (rect.x + rect.width)) ? rect.x + rect.width : canvas->width;
    size_t max_y = (canvas->height > (rect.y + rect.height)) ? rect.y + rect.height : canvas->height;
    uint32_t *pixels = canvas->pixels;
    size_t offset = canvas->offset;
    size_t stride = canvas->stride;

    for (size_t y = rect.y; y < max_y; y++)
    {
        for (size_t x = rect.x; x < max_x; x++)
        {
            PIXEL_AT(pixels, x, y, stride, offset) = color;
        }
    }
}

void pixie_draw_disc(Pixie_Canvas *canvas, size_t cx, size_t cy, size_t radius, uint32_t color)
{
    int min_x = (((int)cx - (int)radius) < 0) ? 0 : cx - radius;
    int min_y = (((int)cy - (int)radius) < 0) ? 0 : cy - radius;
    int max_x = (canvas->width > (cx + radius)) ? cx + radius : canvas->width;
    int max_y = (canvas->height > (cy + radius)) ? cy + radius : canvas->height;
    uint32_t *pixels = canvas->pixels;
    int offset = canvas->offset;
    int stride = canvas->width;
    int r_sq = radius * radius;

    for (int y = min_y; y < max_y; y++)
    {
        int dy = y-cy;
        int dy_sq = dy * dy;  
        for (int x = min_x; x < max_x; x++)
        {
            int dx = x - cx;
            if ((dx * dx + dy_sq) < r_sq)
            PIXEL_AT(pixels, x, y, stride, offset) = color;
        }
    }
}

void pixie_draw_disc_aa(Pixie_Canvas *canvas, size_t cx, size_t cy, size_t radius, size_t aa)
{

}

void pixie_draw_circle(Pixie_Canvas *canvas, size_t cx, size_t cy, size_t radius, uint32_t color)
{
    int x = 0, y = radius;
    int xc = cx, yc = cy;
    uint32_t *pixels = canvas->pixels;
    int width = canvas->width;
    int height = canvas->height;
    int offset = canvas->offset;
    int stride = canvas->stride;

    int p = 3 - 2 * (int)radius;

    do 
    {
        pixie_try_set_pixel_int(pixels, width, height, stride, offset, xc + x, yc + y, color);
        pixie_try_set_pixel_int(pixels, width, height, stride, offset, xc - x, yc + y, color);
        pixie_try_set_pixel_int(pixels, width, height, stride, offset, xc + x, yc - y, color);
        pixie_try_set_pixel_int(pixels, width, height, stride, offset, xc - x, yc - y, color);
        pixie_try_set_pixel_int(pixels, width, height, stride, offset, xc + y, yc + x, color);
        pixie_try_set_pixel_int(pixels, width, height, stride, offset, xc - y, yc + x, color);
        pixie_try_set_pixel_int(pixels, width, height, stride, offset, xc + y, yc - x, color);
        pixie_try_set_pixel_int(pixels, width, height, stride, offset, xc - y, yc - x, color);

        x++;
        if(p > 0)
        {
            y--;
            p += 4*(x - y) + 10;
        }
        else
        {
            p += 4*x + 6;
        }
    } while (x < y);
}

void pixie_draw_ellipse(Pixie_Canvas *canvas, size_t cx, size_t cy, size_t a, size_t b)
{

}

void _draw_line_low(Pixie_Canvas *canvas, int x1, int y1, int x2, int y2, uint32_t color)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    int yi = 1;
    if (dy < 0)
    {
        yi = -1;
        dy = -dy;
    }
    int D = (2 * dy) - dx;
    int y = y1;
    uint32_t *pixels = canvas->pixels;
    int stride = canvas->stride;
    int offset = canvas->offset;
    int x_max = (int)canvas->width;
    int y_max = (int)canvas->height;

    for (int x = x1; x < x2; x++)
    {
        if ((x > -1) && (x < x_max) && (y > -1) && (y < y_max)) 
        {
            PIXEL_AT(pixels, x, y, stride, offset) = color;
        }

        if (D > 0)
        {
            y += yi;
            D += 2 * (dy - dx);
        }
        else
            D += 2*dy;
    }
}

void _draw_line_high(Pixie_Canvas *canvas, int x1, int y1, int x2, int y2, uint32_t color)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    int xi = 1;
    if (dx < 0)
    {
        xi = -1;
        dx = -dx;
    }

    int D = (2 * dx) - dy;
    int x = x1;
    uint32_t *pixels = canvas->pixels;
    int stride = canvas->stride;
    int offset = canvas->offset;
    int x_max = (int)canvas->width;
    int y_max = (int)canvas->height;

    for (int y = y1; y < y2; y++)
    {
        if ((x > -1) && (x < x_max) && (y > -1) && (y < y_max)) 
        {
            PIXEL_AT(pixels, x, y, stride, offset) = color;
        }
        
        if (D > 0)
        {
            x += xi;
            D += (2 * (dx - dy));
        }
        else
            D += 2*dx;
    }
}

void pixie_line_unsafe(Pixie_Canvas *canvas, size_t x1, size_t y1, size_t x2, size_t y2, uint32_t color)
{
    // Bresenham
    int ix1 = x1;
    int ix2 = x2;
    int iy1 = y1;
    int iy2 = y2;

    if (abs(iy2 - iy1) < abs(ix2 - ix1))
    {

        if (ix2 < ix1)
            _draw_line_low(canvas, ix2, iy2, ix1, iy1, color);
        else
            _draw_line_low(canvas, ix1, iy1, ix2, iy2, color);
    }
    else
    {

        if (iy1 < iy2)
            _draw_line_high(canvas, ix2, iy2, ix1, iy1, color);
        else
            _draw_line_high(canvas, ix1, iy1, ix2, iy2, color);
    }
}

void pixie_line_aa(Pixie_Canvas canvas, size_t x1, size_t y1, size_t x2, size_t y2, size_t aa)
{

}
