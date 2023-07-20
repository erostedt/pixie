#include "canvas.h"
#include "stdlib.h"
#include "assert.h"
#include "stdio.h"
#include "math.h"


Pixie_Rect pixie_rect_new(Pixie_Point top_left, size_t width, size_t height)
{
    return (Pixie_Rect){.top_left=top_left, .width=width, .height=height};
}


Pixie_Canvas pixie_canvas_new(size_t width, size_t height)
{
    uint32_t *pixels = calloc(width * height, sizeof(uint32_t));
    assert(pixels != NULL);
    return (Pixie_Canvas){ .width=width, .height=height, .stride=width, .pixels=pixels };
}


Pixie_Canvas pixie_subcanvas_new(Pixie_Canvas *canvas, Pixie_Rect region)
{
    assert(((region.top_left.x + region.width) < canvas->width) && ((region.top_left.y + region.height) < canvas->height));
    uint32_t *data = canvas->pixels + (region.top_left.y * canvas->stride + region.top_left.x);
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
    size_t rx = rect.top_left.x, ry = rect.top_left.y; 
    size_t max_x = (canvas->width > (rx + rect.width)) ? rx + rect.width : canvas->width;
    size_t max_y = (canvas->height > (ry + rect.height)) ? ry + rect.height : canvas->height;
    uint32_t *pixels = canvas->pixels;
    size_t stride = canvas->stride;

    for (size_t y = ry; y < max_y; y++)
    {
        for (size_t x = rx; x < max_x; x++)
        {
            PIXEL_AT(pixels, x, y, stride) = color;
        }
    }
}


void pixie_draw_rectangle(Pixie_Canvas *canvas, Pixie_Rect rect, uint32_t color)
{       
    size_t rx = rect.top_left.x, ry = rect.top_left.y; 
    size_t max_x = (canvas->width > (rx + rect.width)) ? rx + rect.width : canvas->width;
    size_t max_y = (canvas->height > (ry + rect.height)) ? ry + rect.height : canvas->height;
    uint32_t *pixels = canvas->pixels;
    size_t stride = canvas->stride;

    for (size_t y = ry; y < max_y; y++)
    {
        PIXEL_AT(pixels, rx, y, stride) = color;
        PIXEL_AT(pixels, max_x, y, stride) = color;
    }
    
    for (size_t x = rx; x < max_x; x++)
    {
        PIXEL_AT(pixels, x, ry, stride) = color;
        PIXEL_AT(pixels, x, max_y, stride) = color;
    }
}


void pixie_draw_filled_square(Pixie_Canvas *canvas, Pixie_Point top_left, size_t width, uint32_t color)
{
    pixie_draw_filled_rectangle(canvas, pixie_rect_new(top_left, width, width), color);
}


void pixie_draw_square(Pixie_Canvas *canvas, Pixie_Point top_left, size_t width, uint32_t color)
{
    pixie_draw_rectangle(canvas, pixie_rect_new(top_left, width, width), color);
}


void pixie_draw_filled_circle(Pixie_Canvas *canvas, Pixie_Point center, size_t radius, uint32_t color)
{
    int x = 0, y = radius;
    int xc = center.x, yc = center.y;
    int p = 3 - 2 * (int)radius;

    do 
    {
        pixie_draw_line(canvas, (Pixie_Point){xc + x, yc + y}, (Pixie_Point){xc - x, yc + y}, color);
        pixie_draw_line(canvas, (Pixie_Point){xc - x, yc + y}, (Pixie_Point){xc - x, yc - y}, color);
        pixie_draw_line(canvas, (Pixie_Point){xc - x, yc - y}, (Pixie_Point){xc + x, yc - y}, color);
        pixie_draw_line(canvas, (Pixie_Point){xc + x, yc - y}, (Pixie_Point){xc + x, yc + y}, color);

        pixie_draw_line(canvas, (Pixie_Point){xc + y, yc + x}, (Pixie_Point){xc - y, yc + x}, color);
        pixie_draw_line(canvas, (Pixie_Point){xc - y, yc + x}, (Pixie_Point){xc - y, yc - x}, color);
        pixie_draw_line(canvas, (Pixie_Point){xc - y, yc - x}, (Pixie_Point){xc + y, yc - x}, color);
        pixie_draw_line(canvas, (Pixie_Point){xc + y, yc - x}, (Pixie_Point){xc + y, yc + x}, color);
        
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


void pixie_draw_circle(Pixie_Canvas *canvas, Pixie_Point center, size_t radius, uint32_t color)
{
    size_t cx = center.x, cy = center.y;
    assert(((cx + radius) < canvas->width) && ((cy + radius) < canvas->height));
    assert((cx >= radius) && (cy >= radius));
    
    int x = 0, y = radius;
    int xc = center.x, yc = center.y;
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


void pixie_draw_ellipse(Pixie_Canvas *canvas, Pixie_Point center, size_t a, size_t b, uint32_t color)
{
    size_t cx = center.x, cy = center.y;
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


void pixie_draw_filled_ellipse(Pixie_Canvas *canvas, Pixie_Point center, size_t a, size_t b, uint32_t color)
{
    size_t cx = center.x, cy = center.y;
    assert(((cx + a) < canvas->width) && ((cy + b) < canvas->height));
    assert((cx >= a) && (cy >= b));

    int xc = cx, yc = cy, rx = a, ry = b;
    int x = 0;
    int y = ry;
 
    // Initial decision parameter of region 1
    float d1 = (ry * ry) - (rx * rx * ry) + (0.25 * rx * rx);
    float dx = 2 * ry * ry * x;
    float dy = 2 * rx * rx * y;
 
    while (dx < dy) 
    {
        pixie_draw_line(canvas, (Pixie_Point){xc + x, yc + y}, (Pixie_Point){xc - x, yc + y}, color);
        pixie_draw_line(canvas, (Pixie_Point){xc - x, yc + y}, (Pixie_Point){xc - x, yc - y}, color);
        pixie_draw_line(canvas, (Pixie_Point){xc - x, yc - y}, (Pixie_Point){xc + x, yc - y}, color);
        pixie_draw_line(canvas, (Pixie_Point){xc + x, yc - y}, (Pixie_Point){xc + x, yc + y}, color);
 
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
        pixie_draw_line(canvas, (Pixie_Point){xc + x, yc + y}, (Pixie_Point){xc - x, yc + y}, color);
        pixie_draw_line(canvas, (Pixie_Point){xc - x, yc + y}, (Pixie_Point){xc - x, yc - y}, color);
        pixie_draw_line(canvas, (Pixie_Point){xc - x, yc - y}, (Pixie_Point){xc + x, yc - y}, color);
        pixie_draw_line(canvas, (Pixie_Point){xc + x, yc - y}, (Pixie_Point){xc + x, yc + y}, color);
 
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

void _draw_bottom_flat_triangle(Pixie_Canvas *canvas, Pixie_Point p1, Pixie_Point p2, Pixie_Point p3, uint32_t color)
{
    float invslope1 = ((float)p2.x - (float)p1.x) / ((float)p2.y - (float)p1.y);
    float invslope2 = ((float)p3.x - (float)p1.x) / ((float)p3.y - (float)p1.y);

    float curx1 = (float)p1.x;
    float curx2 = (float)p1.x;

    for (size_t sy = p1.y; sy <= p2.y; sy++)
    {
        pixie_draw_line(canvas, (Pixie_Point){(size_t)curx1, sy}, (Pixie_Point){(size_t)curx2, sy}, color);
        curx1 += invslope1;
        curx2 += invslope2; 
    }
}

void _draw_top_flat_triangle(Pixie_Canvas *canvas, Pixie_Point p1, Pixie_Point p2, Pixie_Point p3, uint32_t color)
{
    float invslope1 = ((float)p3.x - (float)p1.x) / ((float)p3.y - (float)p1.y);
    float invslope2 = ((float)p3.x - (float)p2.x) / ((float)p3.y - (float)p2.y);

    float curx1 = (float)p3.x;
    float curx2 = (float)p3.x;

    for (size_t sy = p3.y; sy > p1.y; sy--)
    {
        pixie_draw_line(canvas, (Pixie_Point){curx1, sy}, (Pixie_Point){curx2, sy}, color);
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}


void pixie_draw_triangle(Pixie_Canvas *canvas, Pixie_Point p1, Pixie_Point p2, Pixie_Point p3, uint32_t color)
{
    pixie_draw_line(canvas, p1, p2, color);
    pixie_draw_line(canvas, p1, p3, color);
    pixie_draw_line(canvas, p2, p3, color);
}

void pixie_draw_filled_triangle(Pixie_Canvas *canvas, Pixie_Point p1, Pixie_Point p2, Pixie_Point p3, uint32_t color)
{
    // http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
    if (p1.y > p2.y) swap_points(&p1, &p2);
    if (p1.y > p3.y) swap_points(&p1, &p3);
    if (p2.y > p3.y) swap_points(&p2, &p3);
    
    if (p2.y == p3.y)
    {
        _draw_bottom_flat_triangle(canvas, p1, p2, p3, color);
    }
    
    else if (p1.y == p2.y)
    {
        _draw_top_flat_triangle(canvas, p1, p2, p3, color);
    }
    else
    {
        Pixie_Point p4 = 
        {
            .x=(size_t)((float)p1.x + ((float)(p2.y - p1.y) / (float)(p3.y - p1.y)) * ((float)p3.x - (float)p1.x)),
            .y=p2.y,
        };

        _draw_bottom_flat_triangle(canvas, p1, p2, p4, color);
        _draw_top_flat_triangle(canvas, p2, p4, p3, color);
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

void pixie_draw_line(Pixie_Canvas *canvas, Pixie_Point p1, Pixie_Point p2, uint32_t color)
{
    // Bresenham
    int ix1 = p1.x, ix2 = p2.x;
    int iy1 = p1.y, iy2 = p2.y;

    if (abs(iy2 - iy1) < abs(ix2 - ix1))
    {

        if (ix2 < ix1)
            _draw_line_low(canvas, ix2, iy2, ix1, iy1, color);
        else
            _draw_line_low(canvas, ix1, iy1, ix2, iy2, color);
    }
    else
    {

        if (iy2 < iy1)
            _draw_line_high(canvas, ix2, iy2, ix1, iy1, color);
        else
            _draw_line_high(canvas, ix1, iy1, ix2, iy2, color);
    }
}

void pixie_draw_thick_line(Pixie_Canvas *canvas, Pixie_Point p1, Pixie_Point p2, size_t thickness, uint32_t color)
{
    if (thickness == 0) return;
    if (thickness == 1) 
    {
        pixie_draw_line(canvas, p1, p2, color);
        return;
    }

    if (p1.x == p2.x)
    {
        if (p2.y > p1.y)
            pixie_draw_thick_vline(canvas, p1.x, p1.y, p2.y, thickness, color);
        else
            pixie_draw_thick_vline(canvas, p1.x, p2.y, p1.y, thickness, color);
        
        return;
    }

    float x1 = p1.x, x2 = p2.x, y1 = p1.y, y2 = p2.y;
    float dx = x2 - x1, dy = y2 - y1;
    float adx = (dx > 0) ? dx : -dx;
    float ady = (dy > 0) ? dy : -dy;

    float slope = dy / dx;;
    if (slope < 1.0)
    {
        float wy = (thickness-1) * sqrt((dx*dx) + (dy*dy)) / (2*adx);
        for(size_t i = 0; i < wy; i++)
        {
            pixie_draw_line(canvas, (Pixie_Point){x1, y1-i}, (Pixie_Point){x2, y2-i}, color);
            pixie_draw_line(canvas, (Pixie_Point){x1, y1+i}, (Pixie_Point){x2, y2+i}, color);
        }
    }
     else
    {
        float wx = (thickness-1) * sqrt((dx*dx) + (dy*dy)) / (2*ady);
        for(size_t i =0; i < wx; i++)
        {
            pixie_draw_line(canvas, (Pixie_Point){x1-i, y1}, (Pixie_Point){x2, y2}, color);
            pixie_draw_line(canvas, (Pixie_Point){x1+i, y1}, (Pixie_Point){x2+i, y2}, color);
        }
    }
}


void pixie_draw_vline(Pixie_Canvas *canvas, size_t x, size_t ymin, size_t ymax, uint32_t color)
{
    assert(ymin <= ymax);
    size_t max_y = (ymax >= canvas->height) ? canvas->height : ymax;
    uint32_t *pixels = canvas->pixels;
    size_t stride = canvas->stride;

    for (size_t y = ymin; y < max_y; y++)
        PIXEL_AT(pixels, x, y, stride) = color;
}


void pixie_draw_hline(Pixie_Canvas *canvas, size_t y, size_t xmin, size_t xmax, uint32_t color)
{
    assert(xmin <= xmax);
    size_t max_x = (xmax >= canvas->width) ? canvas->width : xmax;
    uint32_t *pixels = canvas->pixels;
    size_t stride = canvas->stride;

    for (size_t x = xmin; x < max_x; x++)
        PIXEL_AT(pixels, x, y, stride) = color;
}


void pixie_draw_thick_vline(Pixie_Canvas *canvas, size_t x , size_t ymin, size_t ymax, size_t thickness, uint32_t color)
{
    // fix so that n and n+1 is not the same.
    assert(x < canvas->width);
    assert(ymax >= ymin);
    assert((thickness/2) < ymin + 1);
    assert(((thickness/2) +  ymax) < canvas->height);
    if (thickness == 0) return;
    
    pixie_draw_vline(canvas, x, ymin, ymax, color);
    size_t half_thickness = thickness/2 + 1;
    for (size_t dx = 1; dx < half_thickness; dx++ )
    {
        pixie_draw_vline(canvas, x + dx, ymin, ymax, color);
        pixie_draw_vline(canvas, x - dx - 1, ymin, ymax, color);

    }

}