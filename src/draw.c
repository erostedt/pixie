#include "draw.h"
#include "assert.h"
#include "math.h"

static inline void pixie_blend_colors_avg(uint32_t *rgba1, uint32_t rgba2)
{
    uint32_t r1 = PIXIE_RED(*rgba1);
    uint32_t g1 = PIXIE_GREEN(*rgba1);
    uint32_t b1 = PIXIE_BLUE(*rgba1);
    uint32_t a1 = PIXIE_ALPHA(*rgba1);

    uint32_t r2 = PIXIE_RED(rgba2);
    uint32_t g2 = PIXIE_GREEN(rgba2);
    uint32_t b2 = PIXIE_BLUE(rgba2);
    uint32_t a2 = PIXIE_ALPHA(rgba2);

    r1 = (r1 * (255 - a2) + r2 * a2) / 255;
    if (r1 > 255)
        r1 = 255;
    g1 = (g1 * (255 - a2) + g2 * a2) / 255;
    if (g1 > 255)
        g1 = 255;
    b1 = (b1 * (255 - a2) + b2 * a2) / 255;
    if (b1 > 255)
        b1 = 255;

    *rgba1 = PIXIE_RGBA(r1, g1, b1, a1);
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
            pixels[y * stride + x] = color;
        }

        if (D > 0)
        {
            y += yi;
            D += 2 * (dy - dx);
        }
        else
            D += 2 * dy;
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
            pixels[y * stride + x] = color;
        }

        if (D > 0)
        {
            x += xi;
            D += (2 * (dx - dy));
        }
        else
            D += 2 * dx;
    }
}

void _swap_ints(int *int1, int *int2)
{
    int temp = *int1;
    *int1 = *int2;
    *int2 = temp;
}

void _swap_size_ts(size_t *s1, size_t *s2)
{
    size_t temp = *s1;
    *s1 = *s2;
    *s2 = temp;
}

void _pixie_bresenham(Pixie_Canvas *canvas, Pixie_Point p1, Pixie_Point p2, uint32_t color)
{
    // Bresenham
    int ix1 = p1.x, ix2 = p2.x;
    int iy1 = p1.y, iy2 = p2.y;

    if (abs(iy2 - iy1) < abs(ix2 - ix1))
    {
        if (ix2 < ix1)
        {
            _swap_ints(&ix1, &ix2);
            _swap_ints(&iy1, &iy2);
        }
        _draw_line_low(canvas, ix1, iy1, ix2, iy2, color);
    }
    else
    {
        if (iy2 < iy1)
        {
            _swap_ints(&ix1, &ix2);
            _swap_ints(&iy1, &iy2);
        }
        _draw_line_high(canvas, ix1, iy1, ix2, iy2, color);
    }
}

void pixie_draw_filled_rectangle(Pixie_Canvas *canvas, Pixie_Rect rect, uint32_t color)
{
    size_t rx = rect.x, ry = rect.y;
    size_t max_x = (canvas->width > (rx + rect.w)) ? rx + rect.w : canvas->width;
    size_t max_y = (canvas->height > (ry + rect.h)) ? ry + rect.h : canvas->height;
    uint32_t *pixels = canvas->pixels;
    size_t stride = canvas->stride;

    for (size_t y = ry; y < max_y; y++)
    {
        size_t ys = y * stride;
        for (size_t x = rx; x < max_x; x++)
            pixels[ys + x] = color;
    }
}

void pixie_draw_hollow_rectangle(Pixie_Canvas *canvas, Pixie_Rect rect, uint32_t color)
{
    size_t rx = rect.x, ry = rect.y;
    size_t max_x = (canvas->width - 1 > (rx + rect.w)) ? rx + rect.w : canvas->width - 1;
    size_t max_y = (canvas->height - 1 > (ry + rect.h)) ? ry + rect.h : canvas->height - 1;
    uint32_t *pixels = canvas->pixels;
    size_t stride = canvas->stride;

    for (size_t y = ry; y <= max_y; y++)
    {
        size_t ys = y * stride;
        pixels[ys + rx] = color;
        pixels[ys + max_x] = color;
    }

    size_t ysmin = ry * stride;
    size_t ysmax = max_y * stride;
    for (size_t x = rx; x <= max_x; x++)
    {
        pixels[ysmin + x] = color;
        pixels[ysmax + x] = color;
    }
    pixels[ysmax + max_x] = color;
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
        _pixie_bresenham(canvas, POINT(xc + x, yc + y), POINT(xc - x, yc + y), color);
        _pixie_bresenham(canvas, POINT(xc - x, yc + y), POINT(xc - x, yc - y), color);
        _pixie_bresenham(canvas, POINT(xc - x, yc - y), POINT(xc + x, yc - y), color);
        _pixie_bresenham(canvas, POINT(xc + x, yc - y), POINT(xc + x, yc + y), color);

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
        _pixie_bresenham(canvas, POINT(xc + x, yc + y), POINT(xc - x, yc + y), color);
        _pixie_bresenham(canvas, POINT(xc - x, yc + y), POINT(xc - x, yc - y), color);
        _pixie_bresenham(canvas, POINT(xc - x, yc - y), POINT(xc + x, yc - y), color);
        _pixie_bresenham(canvas, POINT(xc + x, yc - y), POINT(xc + x, yc + y), color);

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

void pixie_draw_hollow_ellipse(Pixie_Canvas *canvas, Pixie_Point center, size_t a, size_t b, uint32_t color)
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
        pixels[(yc + y) * stride + xc + x] = color;
        pixels[(yc + y) * stride + xc - x] = color;
        pixels[(yc - y) * stride + xc + x] = color;
        pixels[(yc - y) * stride + xc - x] = color;

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
        pixels[(yc + y) * stride + xc + x] = color;
        pixels[(yc + y) * stride + xc - x] = color;
        pixels[(yc - y) * stride + xc + x] = color;
        pixels[(yc - y) * stride + xc - x] = color;

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
        _pixie_bresenham(canvas, POINT((size_t)curx1, sy), POINT((size_t)curx2, sy), color);
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
        _pixie_bresenham(canvas, POINT(curx1, sy), POINT(curx2, sy), color);
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

void pixie_draw_filled_triangle(Pixie_Canvas *canvas, Pixie_Point p1, Pixie_Point p2, Pixie_Point p3, uint32_t color)
{
    // http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
    if (p1.y > p2.y)
        swap_points(&p1, &p2);
    if (p1.y > p3.y)
        swap_points(&p1, &p3);
    if (p2.y > p3.y)
        swap_points(&p2, &p3);

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
        Pixie_Point p4 = {
            .x = (size_t)((float)p1.x + ((float)(p2.y - p1.y) / (float)(p3.y - p1.y)) * ((float)p3.x - (float)p1.x)),
            .y = p2.y,
        };

        _draw_bottom_flat_triangle(canvas, p1, p2, p4, color);
        _draw_top_flat_triangle(canvas, p2, p4, p3, color);
    }
}
void pixie_draw_hollow_triangle(Pixie_Canvas *canvas, Pixie_Point p1, Pixie_Point p2, Pixie_Point p3, uint32_t color)
{
    _pixie_bresenham(canvas, p1, p2, color);
    _pixie_bresenham(canvas, p1, p3, color);
    _pixie_bresenham(canvas, p2, p3, color);
}

void pixie_draw_line(Pixie_Canvas *canvas, Pixie_Point p1, Pixie_Point p2, uint32_t color, size_t thickness)
{
    if (thickness == 0)
        return;
    if (thickness == 1)
    {
        _pixie_bresenham(canvas, p1, p2, color);
        return;
    }

    if (p1.x == p2.x)
    {
        pixie_draw_vline(canvas, p1.x, p1.y, p2.y, color, thickness);
        return;
    }

    float x1 = p1.x, x2 = p2.x, y1 = p1.y, y2 = p2.y;
    float dx = x2 - x1, dy = y2 - y1;
    float adx = (dx > 0) ? dx : -dx;
    float ady = (dy > 0) ? dy : -dy;

    float slope = dy / dx;
    ;
    if (slope < 1.0)
    {
        float wy = (thickness - 1) * sqrt((dx * dx) + (dy * dy)) / (2 * adx);
        for (size_t i = 0; i < wy; i++)
        {
            _pixie_bresenham(canvas, POINT(x1, y1 - i), POINT(x2, y2 - i), color);
            _pixie_bresenham(canvas, POINT(x1, y1 + i), POINT(x2, y2 + i), color);
        }
    }
    else
    {
        float wx = (thickness - 1) * sqrt((dx * dx) + (dy * dy)) / (2 * ady);
        for (size_t i = 0; i < wx; i++)
        {
            _pixie_bresenham(canvas, POINT(x1 - i, y1), POINT(x2, y2), color);
            _pixie_bresenham(canvas, POINT(x1 + i, y1), POINT(x2 + i, y2), color);
        }
    }
}

void _pixie_hline(Pixie_Canvas *canvas, size_t y, size_t xmin, size_t xmax, uint32_t color)
{
    size_t max_x = (xmax >= canvas->width) ? canvas->width : xmax;
    uint32_t *pixels = canvas->pixels;
    size_t ys = canvas->stride * y;

    for (size_t x = xmin; x < max_x; x++)
    {
        pixels[ys + x] = color;
    }
}

void pixie_draw_hline(Pixie_Canvas *canvas, size_t y, size_t xmin, size_t xmax, uint32_t color, size_t thickness)
{
    // fix so that n and n+1 is not the same.
    if (thickness == 0)
        return;
    assert(y < canvas->width);
    if (xmin > xmax)
        _swap_size_ts(&xmin, &xmax);

    assert((thickness / 2) < xmin + 1);
    assert(((thickness / 2) + xmax) < canvas->width);

    _pixie_hline(canvas, y, xmin, xmax, color);
    size_t half_thickness = thickness / 2;
    for (size_t dy = 1; dy < half_thickness; dy++)
    {
        _pixie_hline(canvas, y + dy, xmin, xmax, color);
        _pixie_hline(canvas, y - dy, xmin, xmax, color);
    }
}

void _pixie_vline(Pixie_Canvas *canvas, size_t x, size_t ymin, size_t ymax, uint32_t color)
{
    size_t max_y = (ymax >= canvas->height) ? canvas->height : ymax;
    uint32_t *pixels = canvas->pixels;
    size_t stride = canvas->stride;

    for (size_t y = ymin; y < max_y; y++)
    {
        pixels[y * stride + x] = color;
    }
}

void pixie_draw_vline(Pixie_Canvas *canvas, size_t x, size_t ymin, size_t ymax, uint32_t color, size_t thickness)
{
    // fix so that n and n+1 is not the same.
    if (thickness == 0)
        return;
    assert(x < canvas->width);
    if (ymin > ymax)
        _swap_size_ts(&ymin, &ymax);
    assert((thickness / 2) < ymin + 1);
    assert(((thickness / 2) + ymax) < canvas->height);

    _pixie_vline(canvas, x, ymin, ymax, color);
    size_t half_thickness = thickness / 2;
    for (size_t dx = 1; dx < half_thickness; dx++)
    {
        _pixie_vline(canvas, x + dx, ymin, ymax, color);
        _pixie_vline(canvas, x - dx, ymin, ymax, color);
    }
}
