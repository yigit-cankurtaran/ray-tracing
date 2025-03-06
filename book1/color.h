#ifndef COLOR_H
#define COLOR_H

#include "vec.h"

using color = vec3;
// colors as 3d vectors, x=red y=green z=blue

void write_color(std::ostream &out, const color &pixel_color)
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // translating the [0, 1] component values to the byte range [0, 255]
    int rbyte = int(255.999 * r); // 255.999 is a trick to make sure we avoid rounding errors
    // when we use this for colors we'll pass in vals between 0-1 (by intensity)
    // but ppm and such want 8 bits of num values (up to 255) so we multiply by 255
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    // write out the color componnets

    out << rbyte << ' ' << gbyte << ' ' << bbyte << std::endl;
}

#endif