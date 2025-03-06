#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

// std using
using std::make_shared;
using std::shared_ptr;

// constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// util funcs

inline double degrees_to_radians(double degrees)
// inline = expands in line when called instead of sent back to the object
{
    return degrees * pi / 180.0;
}

// common headers

#include "color.h"
#include "ray.h"
#include "vec.h"
#include "interval.h"

#endif