#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include <pixie.h>

#define ARRAY_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))
#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480

typedef struct
{
    double x, y, z;
} Vec3;

typedef struct
{
    Vec3 center;
    double radius;
    RGBA32 color;
} Sphere;

typedef struct
{
    Vec3 min;
    Vec3 max;
    RGBA32 color;
} Cube;

typedef struct
{
    Sphere *spheres;
    size_t sphere_count;
    Cube *cubes;
    size_t cube_count;
} Scene;

Vec3 vec3(double x, double y, double z)
{
    return (Vec3){x, y, z};
}

Vec3 sub(Vec3 a, Vec3 b)
{
    return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

double dot(Vec3 a, Vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 normalize(Vec3 v)
{
    double len = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return vec3(v.x / len, v.y / len, v.z / len);
}

typedef struct
{
    Vec3 origin;
    Vec3 direction;
} Ray;

bool intersect_sphere(Sphere sphere, Ray ray, double *t)
{
    Vec3 oc = sub(ray.origin, sphere.center);
    double a = dot(ray.direction, ray.direction);
    double b = 2.0 * dot(oc, ray.direction);
    double c = dot(oc, oc) - sphere.radius * sphere.radius;
    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0)
    {
        return false;
    }
    else
    {
        *t = (-b - sqrt(discriminant)) / (2.0 * a);
        return true;
    }
}

void swap(double *x, double *y)
{
    double temp = *y;
    *y = *x;
    *x = temp;
}

bool intersect_cube(Cube cube, Ray ray, double *t)
{
    double tmin = (cube.min.x - ray.origin.x) / ray.direction.x;
    double tmax = (cube.max.x - ray.origin.x) / ray.direction.x;

    double tymin = (cube.min.y - ray.origin.y) / ray.direction.y;
    double tymax = (cube.max.y - ray.origin.y) / ray.direction.y;

    double tzmin = (cube.min.z - ray.origin.z) / ray.direction.z;
    double tzmax = (cube.max.z - ray.origin.z) / ray.direction.z;

    if (tmin > tmax)
    {
        swap(&tmin, &tmax);
    }

    if (tymin > tymax)
    {
        swap(&tymin, &tymax);
    }

    if (tzmin > tzmax)
    {
        swap(&tzmin, &tzmax);
    }

    if ((tmin > tymax) || (tymin > tmax))
    {
        return false;
    }

    if (tymin > tmin)
    {
        tmin = tymin;
    }
    if (tymax < tmax)
    {
        tmax = tymax;
    }

    if ((tmin > tzmax) || (tzmin > tmax))
    {
        return false;
    }

    if (tzmin > tmin)
    {
        tmin = tzmin;
    }

    if (tzmax < tmax)
    {
        tmax = tzmax;
    }

    *t = tmin;
    return true;
}

RGBA32 raytrace(const Scene *scene, Ray ray)
{
    RGBA32 color = PIXIE_RGBA(0, 0, 0, 255);
    double t_min = DBL_MAX;

    double t;
    for (size_t i = 0; i < scene->sphere_count; ++i)
    {
        if (intersect_sphere(scene->spheres[i], ray, &t) && t < t_min)
        {
            t_min = t;
            color = scene->spheres[i].color;
        }
    }

    for (size_t i = 0; i < scene->cube_count; ++i)
    {
        if (intersect_cube(scene->cubes[i], ray, &t) && t < t_min)
        {
            t_min = t;
            color = scene->cubes[i].color;
        }
    }
    return color;
}

void render(PixieCanvas *canvas, const Scene *scene)
{
    Vec3 camera = vec3(0, 0, 0);
    size_t width = canvas->width;
    size_t height = canvas->height;
    double aspect_ratio = (double)width / height;

    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            double u = (2.0 * ((x + 0.5) / width) - 1.0) * aspect_ratio;
            double v = (1.0 - 2.0 * ((y + 0.5) / height));
            Ray ray = {camera, normalize(vec3(u, v, -1))};

            PIXEL_AT(canvas, x, y) = raytrace(scene, ray);
        }
    }
}

int main()
{
    Scene scene;
    Sphere spheres[] = {{{0, 0, -5}, 1, PIXIE_RGBA(255, 0, 0, 255)},
                        {{-2, 0, -6}, 1, PIXIE_RGBA(0, 255, 0, 255)},
                        {{2, 0, -6}, 1, PIXIE_RGBA(0, 0, 255, 255)}};

    Cube cubes[] = {{{-1, -1, -4}, {0, 0, -5}, PIXIE_RGBA(255, 255, 0, 255)},
                    {{1, -1, -5}, {2, 0, -6}, PIXIE_RGBA(0, 255, 255, 255)}};
    scene.spheres = spheres;
    scene.sphere_count = ARRAY_LEN(spheres);
    scene.cubes = cubes;
    scene.cube_count = ARRAY_LEN(cubes);

    PixieCanvas canvas = pixie_canvas_new(IMAGE_WIDTH, IMAGE_HEIGHT);
    render(&canvas, &scene);
    pixie_canvas_save_as_ppm(&canvas, "raytrace.ppm");
}
