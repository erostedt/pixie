#include "canvas.h"
#include "stdlib.h"
#include "assert.h"
#include "stdio.h"


Pixie_Rect pixie_rect_new(size_t x, size_t y, size_t width, size_t height)
{
    return (Pixie_Rect){.x=x, .y=y, .width=width, .height=height};
}


Pixie_Canvas pixie_canvas_new(size_t width, size_t height)
{
    uint32_t *pixels = calloc(width * height, sizeof(uint32_t));
    assert(pixels != NULL);
    return (Pixie_Canvas){ .width=width, .height=height, .stride=width, .pixels=pixels };
}


Pixie_Canvas pixie_subcanvas_new(Pixie_Canvas *canvas, Pixie_Rect region)
{
    assert(((region.x + region.width) < canvas->width) && ((region.y + region.height) < canvas->height));
    uint32_t *data = canvas->pixels + (region.y * canvas->stride + region.x);
    return (Pixie_Canvas) { .width=region.width, .height=region.height, .stride=canvas->stride, .pixels=data };
}


void pixie_canvas_free(Pixie_Canvas *canvas)
{
    canvas->height = canvas->stride = canvas->width = 0;
    free(canvas->pixels);
    canvas = NULL;
}


void pixie_canvas_fill(Pixie_Canvas *canvas, uint32_t color)
{
    uint32_t *pixels = canvas->pixels;
    size_t width = canvas->width;
    size_t height = canvas->height;
    size_t stride = canvas->stride;
    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
            PIXEL_AT(pixels, x, y, stride) = color;
    }
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
    size_t width = canvas->width;
    size_t height = canvas->height;
    
    uint32_t *pixels = canvas->pixels;


    fprintf(f, "P6\n%zu %zu 255\n", width, height);

    for (size_t y = 0; y < height; ++y) 
    {
        for (size_t x = 0; x < width; ++x) 
        {
            uint32_t pixel = PIXEL_AT(pixels, x, y, stride);
            uint8_t buf[3] = {(uint8_t)RED(pixel), (uint8_t)GREEN(pixel), (uint8_t)BLUE(pixel)};
            
            fwrite(buf, sizeof(buf), 1, f);
        }
    }

    fclose(f);
}


void pixie_draw_filled_rectangle(Pixie_Canvas *canvas, Pixie_Rect rect, uint32_t color)
{
    size_t max_x = (canvas->width > (rect.x + rect.width)) ? rect.x + rect.width : canvas->width;
    size_t max_y = (canvas->height > (rect.y + rect.height)) ? rect.y + rect.height : canvas->height;
    uint32_t *pixels = canvas->pixels;
    size_t stride = canvas->stride;

    for (size_t y = rect.y; y < max_y; y++)
    {
        for (size_t x = rect.x; x < max_x; x++)
        {
            PIXEL_AT(pixels, x, y, stride) = color;
        }
    }
}


void pixie_draw_rectangle(Pixie_Canvas *canvas, Pixie_Rect rect, uint32_t color)
{
    size_t max_x = (canvas->width > (rect.x + rect.width)) ? rect.x + rect.width : canvas->width;
    size_t max_y = (canvas->height > (rect.y + rect.height)) ? rect.y + rect.height : canvas->height;
    uint32_t *pixels = canvas->pixels;
    size_t stride = canvas->stride;

    for (size_t y = rect.y; y < max_y; y++)
    {
        PIXEL_AT(pixels, rect.x, y, stride) = color;
        PIXEL_AT(pixels, max_x, y, stride) = color;
    }
    
    for (size_t x = rect.x; x < max_x; x++)
    {
        PIXEL_AT(pixels, x, rect.y, stride) = color;
        PIXEL_AT(pixels, x, max_y, stride) = color;
    }
}


void pixie_draw_filled_circle(Pixie_Canvas *canvas, size_t cx, size_t cy, size_t radius, uint32_t color)
{
    int min_x = (((int)cx - (int)radius) < 0) ? 0 : cx - radius;
    int min_y = (((int)cy - (int)radius) < 0) ? 0 : cy - radius;
    int max_x = (canvas->width > (cx + radius)) ? cx + radius : canvas->width;
    int max_y = (canvas->height > (cy + radius)) ? cy + radius : canvas->height;
    uint32_t *pixels = canvas->pixels;
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
            PIXEL_AT(pixels, x, y, stride) = color;
        }
    }
}


void pixie_draw_circle(Pixie_Canvas *canvas, size_t cx, size_t cy, size_t radius, uint32_t color)
{
    int x = 0, y = radius;
    int xc = cx, yc = cy;
    uint32_t *pixels = canvas->pixels;
    int width = canvas->width;
    int height = canvas->height;
    int stride = canvas->stride;

    int p = 3 - 2 * (int)radius;

    do 
    {
        pixie_draw_pixel_int(pixels, width, height, stride, xc + x, yc + y, color);
        pixie_draw_pixel_int(pixels, width, height, stride, xc - x, yc + y, color);
        pixie_draw_pixel_int(pixels, width, height, stride, xc + x, yc - y, color);
        pixie_draw_pixel_int(pixels, width, height, stride, xc - x, yc - y, color);
        pixie_draw_pixel_int(pixels, width, height, stride, xc + y, yc + x, color);
        pixie_draw_pixel_int(pixels, width, height, stride, xc - y, yc + x, color);
        pixie_draw_pixel_int(pixels, width, height, stride, xc + y, yc - x, color);
        pixie_draw_pixel_int(pixels, width, height, stride, xc - y, yc - x, color);

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


void pixie_draw_filled_ellipse(Pixie_Canvas *canvas, size_t cx, size_t cy, size_t a, size_t b, uint32_t color)
{
    // May overflow, probably exists better way to do this.
    assert(((cx + a) < canvas->width) && ((cy + b) < canvas->height));
    assert((cx >= a) && (cy >= b));
    uint32_t *pixels = canvas->pixels;
    size_t stride = canvas->stride;
    size_t xmin = cx - a;
    size_t xmax = cx + a;
    size_t ymin = cy - b;
    size_t ymax = cy + b;
    size_t a2 = a*a;
    size_t b2 = b*b;
    size_t a2b2 = a2 * b2; // This may overflow for large a, b.

    for(size_t y = ymin; y < cy; y++) 
    {
        size_t dy = (cy - y);
        size_t dy2a2 = dy*dy*a2;
        for(size_t x = xmin; x < cx; x++) 
        {
            size_t dx = (cx - x);
            if(dx*dx*b2 + dy2a2 <= a2b2)
                PIXEL_AT(pixels, x, y, stride) = color;
        }  

        for(size_t x = xmax-1; x >=cx; x--)
        {
            size_t dx = (x - cx);
            if(dx*dx*b2 + dy2a2 <= a2b2)
                PIXEL_AT(pixels, x, y, stride) = color;
        }
    } 
    
    for(size_t y = ymax-1; y >= cy; y--) 
    {
        size_t dy = (y - cy);
        size_t dy2a2 = dy*dy*a2;
        for(size_t x = xmin; x < cx; x++) 
        {
            size_t dx = (cx - x);
            if(dx*dx*b2 + dy2a2 <= a2b2)
                PIXEL_AT(pixels, x, y, stride) = color;
        }  

        for(size_t x = xmax-1; x >=cx; x--)
        {
            size_t dx = (x - cx);
            if(dx*dx*b2 + dy2a2 <= a2b2)
                PIXEL_AT(pixels, x, y, stride) = color;
        }
    } 
}


void pixie_draw_ellipse(Pixie_Canvas *canvas, size_t cx, size_t cy, size_t a, size_t b, uint32_t color)
{
    assert(((cx + a) < canvas->width) && ((cy + b) < canvas->height));
    assert((cx >= a) && (cy >= b));

    int xc = cx, yc = cy, rx = a, ry = b;
    int x = 0;
    int y = ry;
 
    // Initial decision parameter of region 1
    float d1 = (ry * ry) - (rx * rx * ry) + (0.25 * rx * rx);
    float dx = 2 * ry * ry * x;
    float dy = 2 * rx * rx * y;
    uint32_t *pixels = canvas->pixels;
    size_t stride = canvas->stride;
 
    while (dx < dy) 
    {
        PIXEL_AT(pixels, xc + x, yc + y, stride) = color;
        PIXEL_AT(pixels, xc - x, yc + y, stride) = color;
        PIXEL_AT(pixels, xc + x, yc - y, stride) = color;
        PIXEL_AT(pixels, xc - x, yc - y, stride) = color;
 
        if (d1 < 0) 
        {
            x++;
            dx = dx + (2 * ry * ry);
            d1 = d1 + dx + (ry * ry);
        }
        else 
        {
            x++;
            y--;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            d1 = d1 + dx - dy + (ry * ry);
        }
    }
 
    float d2 = ((ry * ry) * ((x + 0.5) * (x + 0.5))) + ((rx * rx) * ((y - 1) * (y - 1))) - (rx * rx * ry * ry);
    while (y >= 0) 
    {
        PIXEL_AT(pixels, xc + x, yc + y, stride) = color; 
        PIXEL_AT(pixels, xc - x, yc + y, stride) = color; 
        PIXEL_AT(pixels, xc + x, yc - y, stride) = color; 
        PIXEL_AT(pixels, xc - x, yc - y, stride) = color; 
 
        if (d2 > 0) 
        {
            y--;
            dy = dy - (2 * rx * rx);
            d2 = d2 + (rx * rx) - dy;
        }
        else 
        {
            y--;
            x++;
            dx = dx + (2 * ry * ry);
            dy = dy - (2 * rx * rx);
            d2 = d2 + dx - dy + (rx * rx);
        }
    }
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
    int x_max = (int)canvas->width;
    int y_max = (int)canvas->height;

    for (int x = x1; x < x2; x++)
    {
        if ((x > -1) && (x < x_max) && (y > -1) && (y < y_max)) 
        {
            PIXEL_AT(pixels, x, y, stride) = color;
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
    int x_max = (int)canvas->width;
    int y_max = (int)canvas->height;

    for (int y = y1; y < y2; y++)
    {
        if ((x > -1) && (x < x_max) && (y > -1) && (y < y_max)) 
        {
            PIXEL_AT(pixels, x, y, stride) = color;
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

void pixie_draw_line(Pixie_Canvas *canvas, size_t x1, size_t y1, size_t x2, size_t y2, uint32_t color)
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
