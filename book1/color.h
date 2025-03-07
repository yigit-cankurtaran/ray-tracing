#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec.h"

using color = vec3;
// colors as 3d vectors, x=red y=green z=blue

inline double linear_to_gamma(double linear_component)
{
    if (linear_component > 0)
        return std::sqrt(linear_component);
    return 0;
}

void write_color(std::ostream &out, const color &pixel_color)
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    static const interval intensity(0.000, 0.999);
    // translating the [0, 1] component values to the byte range [0, 255]
    int rbyte = int(256 * intensity.clamp(r));
    // ppm and such want 8 bits of num values (up to 255) so we multiply by 256 and clamped r
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    // write out the color componnets

    out << rbyte << ' ' << gbyte << ' ' << bbyte << std::endl;
}

#endif