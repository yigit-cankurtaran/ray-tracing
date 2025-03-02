// 2.1 ppm image format

#include <iostream>

int main () {
    // Image
    int image_width = 256;
    int image_height = 256;

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) { // every pixel in height
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::endl;
        // clog = character log, flush to update in real time
        // doesn't display too often because it's buffered
        // book uses std::flush but i'll use endl
        for (int i = 0; i < image_width; i++) { // every pixel in width
            auto r = double(i) / (image_width-1);
            auto g = double(j) / (image_height-1);
            auto b = 0.0;

            int ir = int(255.999 * r);
            int ig = int(255.999 * g);
            int ib = int(255.999 * b);

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }

    std::clog << "\rDone.\t\n";
}
