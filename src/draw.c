#include "draw.h"
#include "assert.h"
#include "math.h"


void pixie_draw_filled_rectangle(Pixie_RGB_Image *image, Pixie_Rect rect, rgb24 color)
{
    size_t rx = rect.top_left.x, ry = rect.top_left.y; 
    size_t max_x = (image->width > (rx + rect.width)) ? rx + rect.width : image->width;
    size_t max_y = (image->height > (ry + rect.height)) ? ry + rect.height : image->height;
    rgb24 *pixels = image->pixels;
    size_t stride = image->stride;

    for (size_t y = ry; y < max_y; y++)
    {
        for (size_t x = rx; x < max_x; x++)
        {
            PIXEL_AT(pixels, x, y, stride) = color;
        }
    }
}


void pixie_draw_rectangle(Pixie_RGB_Image *image, Pixie_Rect rect, rgb24 color)
{       
    size_t rx = rect.top_left.x, ry = rect.top_left.y; 
    size_t max_x = (image->width > (rx + rect.width)) ? rx + rect.width : image->width;
    size_t max_y = (image->height > (ry + rect.height)) ? ry + rect.height : image->height;
    rgb24 *pixels = image->pixels;
    size_t stride = image->stride;

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


void pixie_draw_filled_square(Pixie_RGB_Image *image, Pixie_Point top_left, size_t width, rgb24 color)
{
    pixie_draw_filled_rectangle(image, (Pixie_Rect){.top_left=top_left, .width=width, .height=width}, color);
}


void pixie_draw_square(Pixie_RGB_Image *image, Pixie_Point top_left, size_t width, rgb24 color)
{
    pixie_draw_rectangle(image, (Pixie_Rect){.top_left=top_left, .width=width, .height=width}, color);
}


void pixie_draw_filled_circle(Pixie_RGB_Image *image, Pixie_Point center, size_t radius, rgb24 color)
{
    int x = 0, y = radius;
    int xc = center.x, yc = center.y;
    int p = 3 - 2 * (int)radius;

    do 
    {
        pixie_draw_line(image, (Pixie_Point){xc + x, yc + y}, (Pixie_Point){xc - x, yc + y}, color);
        pixie_draw_line(image, (Pixie_Point){xc - x, yc + y}, (Pixie_Point){xc - x, yc - y}, color);
        pixie_draw_line(image, (Pixie_Point){xc - x, yc - y}, (Pixie_Point){xc + x, yc - y}, color);
        pixie_draw_line(image, (Pixie_Point){xc + x, yc - y}, (Pixie_Point){xc + x, yc + y}, color);

        pixie_draw_line(image, (Pixie_Point){xc + y, yc + x}, (Pixie_Point){xc - y, yc + x}, color);
        pixie_draw_line(image, (Pixie_Point){xc - y, yc + x}, (Pixie_Point){xc - y, yc - x}, color);
        pixie_draw_line(image, (Pixie_Point){xc - y, yc - x}, (Pixie_Point){xc + y, yc - x}, color);
        pixie_draw_line(image, (Pixie_Point){xc + y, yc - x}, (Pixie_Point){xc + y, yc + x}, color);
        
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


void pixie_draw_circle(Pixie_RGB_Image *image, Pixie_Point center, size_t radius, rgb24 color)
{
    size_t cx = center.x, cy = center.y;
    assert(((cx + radius) < image->width) && ((cy + radius) < image->height));
    assert((cx >= radius) && (cy >= radius));
    
    int x = 0, y = radius;
    int xc = center.x, yc = center.y;
    rgb24 *pixels = image->pixels;
    int width = image->width;
    int height = image->height;
    int stride = image->stride;

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


void pixie_draw_ellipse(Pixie_RGB_Image *image, Pixie_Point center, size_t a, size_t b, rgb24 color)
{
    size_t cx = center.x, cy = center.y;
    assert(((cx + a) < image->width) && ((cy + b) < image->height));
    assert((cx >= a) && (cy >= b));

    int xc = cx, yc = cy, rx = a, ry = b;
    int x = 0;
    int y = ry;
 
    // Initial decision parameter of region 1
    float d1 = (ry * ry) - (rx * rx * ry) + (0.25 * rx * rx);
    float dx = 2 * ry * ry * x;
    float dy = 2 * rx * rx * y;
    rgb24 *pixels = image->pixels;
    size_t stride = image->stride;
 
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


void pixie_draw_filled_ellipse(Pixie_RGB_Image *image, Pixie_Point center, size_t a, size_t b, rgb24 color)
{
    size_t cx = center.x, cy = center.y;
    assert(((cx + a) < image->width) && ((cy + b) < image->height));
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
        pixie_draw_line(image, (Pixie_Point){xc + x, yc + y}, (Pixie_Point){xc - x, yc + y}, color);
        pixie_draw_line(image, (Pixie_Point){xc - x, yc + y}, (Pixie_Point){xc - x, yc - y}, color);
        pixie_draw_line(image, (Pixie_Point){xc - x, yc - y}, (Pixie_Point){xc + x, yc - y}, color);
        pixie_draw_line(image, (Pixie_Point){xc + x, yc - y}, (Pixie_Point){xc + x, yc + y}, color);
 
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
        pixie_draw_line(image, (Pixie_Point){xc + x, yc + y}, (Pixie_Point){xc - x, yc + y}, color);
        pixie_draw_line(image, (Pixie_Point){xc - x, yc + y}, (Pixie_Point){xc - x, yc - y}, color);
        pixie_draw_line(image, (Pixie_Point){xc - x, yc - y}, (Pixie_Point){xc + x, yc - y}, color);
        pixie_draw_line(image, (Pixie_Point){xc + x, yc - y}, (Pixie_Point){xc + x, yc + y}, color);
 
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

void _draw_bottom_flat_triangle(Pixie_RGB_Image *image, Pixie_Point p1, Pixie_Point p2, Pixie_Point p3, rgb24 color)
{
    float invslope1 = ((float)p2.x - (float)p1.x) / ((float)p2.y - (float)p1.y);
    float invslope2 = ((float)p3.x - (float)p1.x) / ((float)p3.y - (float)p1.y);

    float curx1 = (float)p1.x;
    float curx2 = (float)p1.x;

    for (size_t sy = p1.y; sy <= p2.y; sy++)
    {
        pixie_draw_line(image, (Pixie_Point){(size_t)curx1, sy}, (Pixie_Point){(size_t)curx2, sy}, color);
        curx1 += invslope1;
        curx2 += invslope2; 
    }
}

void _draw_top_flat_triangle(Pixie_RGB_Image *image, Pixie_Point p1, Pixie_Point p2, Pixie_Point p3, rgb24 color)
{
    float invslope1 = ((float)p3.x - (float)p1.x) / ((float)p3.y - (float)p1.y);
    float invslope2 = ((float)p3.x - (float)p2.x) / ((float)p3.y - (float)p2.y);

    float curx1 = (float)p3.x;
    float curx2 = (float)p3.x;

    for (size_t sy = p3.y; sy > p1.y; sy--)
    {
        pixie_draw_line(image, (Pixie_Point){curx1, sy}, (Pixie_Point){curx2, sy}, color);
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}


void pixie_draw_triangle(Pixie_RGB_Image *image, Pixie_Point p1, Pixie_Point p2, Pixie_Point p3, rgb24 color)
{
    pixie_draw_line(image, p1, p2, color);
    pixie_draw_line(image, p1, p3, color);
    pixie_draw_line(image, p2, p3, color);
}

void pixie_draw_filled_triangle(Pixie_RGB_Image *image, Pixie_Point p1, Pixie_Point p2, Pixie_Point p3, rgb24 color)
{
    // http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
    if (p1.y > p2.y) swap_points(&p1, &p2);
    if (p1.y > p3.y) swap_points(&p1, &p3);
    if (p2.y > p3.y) swap_points(&p2, &p3);
    
    if (p2.y == p3.y)
    {
        _draw_bottom_flat_triangle(image, p1, p2, p3, color);
    }
    
    else if (p1.y == p2.y)
    {
        _draw_top_flat_triangle(image, p1, p2, p3, color);
    }
    else
    {
        Pixie_Point p4 = 
        {
            .x=(size_t)((float)p1.x + ((float)(p2.y - p1.y) / (float)(p3.y - p1.y)) * ((float)p3.x - (float)p1.x)),
            .y=p2.y,
        };

        _draw_bottom_flat_triangle(image, p1, p2, p4, color);
        _draw_top_flat_triangle(image, p2, p4, p3, color);
    }
}


void _draw_line_low(Pixie_RGB_Image *image, int x1, int y1, int x2, int y2, rgb24 color)
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
    rgb24 *pixels = image->pixels;
    int stride = image->stride;
    int x_max = (int)image->width;
    int y_max = (int)image->height;

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

void _draw_line_high(Pixie_RGB_Image *image, int x1, int y1, int x2, int y2, rgb24 color)
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
    rgb24 *pixels = image->pixels;
    int stride = image->stride;
    int x_max = (int)image->width;
    int y_max = (int)image->height;
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

void pixie_draw_line(Pixie_RGB_Image *image, Pixie_Point p1, Pixie_Point p2, rgb24 color)
{
    // Bresenham
    int ix1 = p1.x, ix2 = p2.x;
    int iy1 = p1.y, iy2 = p2.y;

    if (abs(iy2 - iy1) < abs(ix2 - ix1))
    {

        if (ix2 < ix1)
            _draw_line_low(image, ix2, iy2, ix1, iy1, color);
        else
            _draw_line_low(image, ix1, iy1, ix2, iy2, color);
    }
    else
    {

        if (iy2 < iy1)
            _draw_line_high(image, ix2, iy2, ix1, iy1, color);
        else
            _draw_line_high(image, ix1, iy1, ix2, iy2, color);
    }
}

void pixie_draw_thick_line(Pixie_RGB_Image *image, Pixie_Point p1, Pixie_Point p2, size_t thickness, rgb24 color)
{
    if (thickness == 0) return;
    if (thickness == 1) 
    {
        pixie_draw_line(image, p1, p2, color);
        return;
    }

    if (p1.x == p2.x)
    {
        if (p2.y > p1.y)
            pixie_draw_thick_vline(image, p1.x, p1.y, p2.y, thickness, color);
        else
            pixie_draw_thick_vline(image, p1.x, p2.y, p1.y, thickness, color);
        
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
            pixie_draw_line(image, (Pixie_Point){x1, y1-i}, (Pixie_Point){x2, y2-i}, color);
            pixie_draw_line(image, (Pixie_Point){x1, y1+i}, (Pixie_Point){x2, y2+i}, color);
        }
    }
     else
    {
        float wx = (thickness-1) * sqrt((dx*dx) + (dy*dy)) / (2*ady);
        for(size_t i =0; i < wx; i++)
        {
            pixie_draw_line(image, (Pixie_Point){x1-i, y1}, (Pixie_Point){x2, y2}, color);
            pixie_draw_line(image, (Pixie_Point){x1+i, y1}, (Pixie_Point){x2+i, y2}, color);
        }
    }
}


void pixie_draw_vline(Pixie_RGB_Image *image, size_t x, size_t ymin, size_t ymax, rgb24 color)
{
    assert(ymin <= ymax);
    size_t max_y = (ymax >= image->height) ? image->height : ymax;
    rgb24 *pixels = image->pixels;
    size_t stride = image->stride;

    for (size_t y = ymin; y < max_y; y++)
        PIXEL_AT(pixels, x, y, stride) = color;
}


void pixie_draw_hline(Pixie_RGB_Image *image, size_t y, size_t xmin, size_t xmax, rgb24 color)
{
    assert(xmin <= xmax);
    size_t max_x = (xmax >= image->width) ? image->width : xmax;
    rgb24 *pixels = image->pixels;
    size_t stride = image->stride;

    for (size_t x = xmin; x < max_x; x++)
        PIXEL_AT(pixels, x, y, stride) = color;
}


void pixie_draw_thick_vline(Pixie_RGB_Image *image, size_t x , size_t ymin, size_t ymax, size_t thickness, rgb24 color)
{
    // fix so that n and n+1 is not the same.
    assert(x < image->width);
    assert(ymax >= ymin);
    assert((thickness/2) < ymin + 1);
    assert(((thickness/2) +  ymax) < image->height);
    if (thickness == 0) return;
    
    pixie_draw_vline(image, x, ymin, ymax, color);
    size_t half_thickness = thickness/2 + 1;
    for (size_t dx = 1; dx < half_thickness; dx++ )
    {
        pixie_draw_vline(image, x + dx, ymin, ymax, color);
        pixie_draw_vline(image, x - dx - 1, ymin, ymax, color);

    }

}