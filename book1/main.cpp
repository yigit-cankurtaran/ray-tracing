#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

int main()
{
    hittable_list world;

    // auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    // auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    // // glass
    // auto material_left = make_shared<dielectric>(1.50);
    // // the air inside the glass
    // auto material_bubble = make_shared<dielectric>(1.00 / 1.50);
    // // reflective metal object
    // auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    // world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    // world.add(make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
    // // glass sphere
    // world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    // // same center, smaller radius for the bubble inside glass sphere
    // world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
    // world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    // touching spheres test

    auto R = std::cos(pi / 4);
    auto material_left = make_shared<lambertian>(color(0, 0, 1));
    auto material_right = make_shared<lambertian>(color(1, 0, 0));

    world.add(make_shared<sphere>(point3(-R, 0, -1), R, material_left));
    world.add(make_shared<sphere>(point3(R, 0, -1), R, material_right));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    // handling anti-aliasing through multiple samples per pixel
    cam.samples_per_pixel = 100;
    cam.max_depth = 20;
    cam.vfov = 90;

    cam.render(world);
}
