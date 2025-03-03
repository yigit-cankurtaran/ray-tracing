#include "color.h"
#include "vec.h"
#include "ray.h"

#include <iostream>

color ray_color(const ray &r)
{
    vec3 unit_direction = unit_vector(r.direction()); // normalize ray direction
    // take direction of the ray and make it a unit vector (length of 1), just keeping the direction info
    auto a = 0.5 * (unit_direction.y() + 1.0); // direction.y gives us how much the ray is pointing up or down
    // +1 = ray is straight up -1 = ray is straight down, a is a value from 0(looking down) to 1(looking up)
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0); // 1,1,1 is white, the other is light blue
    // when a is 0 we get 100% white, when it's 1 we get 100% light blue. for things in the middle we get a mix of the two
}

int main()
{
    // Image
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // calculate image height, make sure it's always at least 1
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // camera

    auto focal_length = 1.0;    // distance from camera to the image plane
    auto viewport_height = 2.0; // dimensions of the virtual screen
    auto viewport_width = viewport_height * (double(image_width) / image_height);
    // calculated accordingly to the aspect ratio we want. viewport_width becomes viewport_height * 16 / 9
    auto camera_center = point3(0, 0, 0); // origin for our rays

    // calculate the vectors across the horizontal and down the vertical viewport
    auto viewport_u = vec3(viewport_width, 0, 0);   // horizontal, points to the right, first is x
    auto viewport_v = vec3(0, -viewport_height, 0); // vertical, points downwards, 2nd is y

    // calculate horizontal and vertical delta vectors pixel to pixel
    // delta vectors are the size of a single pixel in the viewport
    auto pixel_delta_u = viewport_u / image_width;  // how much to move right for one pixel
    auto pixel_delta_v = viewport_v / image_height; // how much to move down for one pixel
    // think of it as "one step right, one step down"

    // calculate location of the upper left pixel
    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    // if we go forward in the focal length (z axis is forward) we go to the center of the viewport
    // if we go half of viewport_u to the left, we go to vertical midline of the viewport (bc viewport is both to the left and right)
    // if we go half of viewport_v upwards, we move up from the center of the horizontal midline, reaching the corner
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    // moving half a pixel right and half a pixel down from the top left corner puts us in the center of the first pixel

    // Render

    std::cout << "P3\n"
              << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++)
    { // every pixel in height
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::endl;
        // clog = character log, flush to update in real time
        // doesn't display too often because it's buffered
        // book uses std::flush but i'll use endl
        for (int i = 0; i < image_width; i++)
        { // every pixel in width
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            // calculate each pixel's center's position in 3d space
            auto ray_direction = pixel_center - camera_center;
            // creating a vector (ray) pointing from the camera to the pixel center
            ray r(camera_center, ray_direction); // create the ray starting from the camera and pointing through the current pixel

            color pixel_color = ray_color(r);    // what color the ray "sees"
            write_color(std::cout, pixel_color); // output that color to an image
        }
    }

    std::clog << "\rDone.\t\n";
}
