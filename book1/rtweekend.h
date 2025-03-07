#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <cstdlib>
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

inline double random_double()
{
    // return random real in [0,1)
    return std::rand() / (RAND_MAX + 1.0);

    // never reaches 1.0 bc rand() will always be smaller than RAND_MAX+1.0
}

inline double random_double(double min, double max)
{
    // return random real in [min, max)
    return min + (max - min) * random_double();

    // random_double gives us a value r between [0,1), (max-min) is the range we want
    // (max-min) * r gives us random offset wihin our desired range
    // adding min means we'll always be larger than our min value
}

// common headers

#include "color.h"
#include "ray.h"
#include "vec.h"
#include "interval.h"

#endif