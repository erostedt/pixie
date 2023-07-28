#include "color.h"


Pixie_GrayImage pixie_rgb2gray(Pixie_RGBImage *image)
{
    Pixie_GrayImage gray = pixie_gray_image_new(image->width, image->height);
    rgb24 *rgb_pixels = image->pixels;
    uint8_t *gray_pixels = gray.pixels;
    size_t width = image->width;
    size_t height = image->height;
    size_t stride = image->stride;

    for (size_t y = 0; y < height; y++)
    {
        size_t ys = y * stride;
        for (size_t x = 0; x < width; x++)
        {
            gray_pixels[ys + x] = rgb2gray(rgb_pixels[ys + x]);
        }
    }
    pixie_rgb_image_free(image);
    return gray;
}

Pixie_RGBImage pixie_gray2rgb(Pixie_GrayImage *image)
{
    Pixie_RGBImage rgb = pixie_rgb_image_new(image->width, image->height);
    uint8_t *gray_pixels = image->pixels;
    rgb24 *rgb_pixels = rgb.pixels;
    size_t width = image->width;
    size_t height = image->height;
    size_t stride = image->stride;

    for (size_t y = 0; y < height; y++)
    {
        size_t ys = y * stride;
        for (size_t x = 0; x < width; x++)
        {
            rgb_pixels[ys + x] = gray2rgb(gray_pixels[ys + x]);
        }
    }
    pixie_gray_image_free(image);
    return rgb;
}