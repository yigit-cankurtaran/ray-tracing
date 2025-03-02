// 2.1 ppm image format

#include "color.h"
#include "vec.h"

#include <iostream>

int main()
{
    // Image
    int image_width = 256;
    int image_height = 256;

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
            auto pixel_color = color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0);
            // double(i) / (image_width - 1) creates a normalized value between 0 and 1, i / 255
            // we then pass into the write_color function to get the 8 bit value of the colors
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.\t\n";
}
