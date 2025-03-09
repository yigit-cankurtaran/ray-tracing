#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class camera
{
public:
    // Image
    double aspect_ratio = 16.0 / 9.0;  // ratio of image width over height
    int image_width = 100;             // rendered image width in pixel count
    int samples_per_pixel = 10;        // random samples for each pixel
    int max_depth = 10;                // maximum number of ray bounces
    double vfov = 90;                  // vertical FOV (view angle)
    point3 lookfrom = point3(0, 0, 0); // point camera is looking from
    point3 lookat = point3(0, 0, -1);  // point camera is looking at
    vec3 vup = vec3(0, 1, 0);          // camera-relative "up" direction

    void render(const hittable &world)
    {
        initialize();

        std::cout << "P3\n"
                  << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) // for every pixel in height
        {
            // going left-right-up-down, every pixel in height is a scanline
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::endl;
            for (int i = 0; i < image_width; i++) // for every pixel in width
            {
                color pixel_color(0, 0, 0); // initialized to black

                // multiple samples for the pixel
                for (int sample = 0; sample < samples_per_pixel; sample++)
                {
                    // ray for this sample
                    ray r = get_ray(i, j);
                    // the color the ray sees to our running sum
                    pixel_color += ray_color(r, max_depth, world);
                }
                // average all samples by multiplying by 1/samples_per_pixel
                write_color(std::cout, pixel_samples_scale * pixel_color);
            }
        }
        std::clog << "\rDone.\t";
    }

private:
    int image_height;           // rendered image height
    double pixel_samples_scale; // color scale factor for a sum of pixel samples
    point3 center;              // camera center
    point3 pixel00_loc;         // location of pixel 0,0
    vec3 pixel_delta_u;         // offset of pixel to the right
    vec3 pixel_delta_v;         // offset of pixel below
    vec3 u, v, w;               // camera frame basis vectors
    void initialize()
    {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = lookfrom; // the center will be the start of our view point

        // viewport dimensions

        auto focal_length = (lookfrom - lookat).length();
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta / 2);
        auto viewport_height = 2 * h * focal_length;
        auto viewport_width = viewport_height * (double(image_width) / image_height);

        // u,v,w unit basis vectors for camera coordinate frame

        // the direction the camera is looking at
        w = unit_vector(lookfrom - lookat);
        // "right" in the camera coordinate, crossing up with our look direction gives us right
        u = unit_vector(cross(vup, w));
        // "up" in the camera coordinate system, crossing w and u gives us "up" from the camera's pov
        v = cross(w, u);

        // vectors across the horizontal and down the vertical viewport edges

        auto viewport_u = viewport_width * u;   // vector across viewport horizontal edge
        auto viewport_v = viewport_height * -v; // vector down viewport vertical edge

        // horizontal and vertical delta vectors

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // calculate upper left pixel

        // start at center, move to the direction we're looking at by focal length (distance to virtual screen), move left and up
        auto viewport_upper_left = center - (focal_length * w) - viewport_u / 2 - viewport_v / 2;
        // start at viewport upper left corner, move inward by half a pixel in both directions, center of first pixel
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    // make a camera ray originating from the origin and directed at a sampled point around the pixel i,j
    ray get_ray(int i, int j) const
    {
        // random offset within the pixel
        auto offset = sample_square();

        // exact point to shoot rays through, starts at top left pixel, move right by i + random offset, move down by j + random offset
        auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);
        // ray starts at camera center
        auto ray_origin = center;
        // ray points from camera to sampled point
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vec3 sample_square() const
    {
        // return vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    color ray_color(const ray &r, int depth, const hittable &world) const
    {
        // if we exceed ray bounces, no more light is gathered
        if (depth <= 0)
            return color(0, 0, 0);
        hit_record rec;

        // ignoring hits close to the calculated intersection point
        // this fixes "shadow acne" problem (dark spots or stripes on lit surfaces)
        if (world.hit(r, interval(0.001, infinity), rec))
        {
            ray scattered;     // new direction after scattering
            color attenuation; // how much the ray's color is reduced by material
            // we use arrow notation bc mat is a pointer
            if (rec.mat->scatter(r, rec, attenuation, scattered))
                // if ray has been scattered by material
                // calculate recursively by calling ray color on scattered ray
                // depth - 1 to limit recursion
                return attenuation * ray_color(scattered, depth - 1, world);
            return color(0, 0, 0); // if no scattering return 0
        }

        vec3 unit_direction = unit_vector(r.direction()); // normalize ray direction
        // take direction of the ray and make it a unit vector (length of 1), just keeping the direction info

        auto a = 0.5 * (unit_direction.y() + 1.0); // direction.y gives us how much the ray is pointing up or down
        // +1 = ray is straight up -1 = ray is straight down, a is a value from 0(looking down) to 1(looking up)

        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0); // 1,1,1 is white, the other is light blue
        // when a is 0 we get 100% white, when it's 1 we get 100% light blue. for things in the middle we get a mix of the two
    }
};

#endif