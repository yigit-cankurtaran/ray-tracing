#include <iostream>
#include "ray.h"

int main()
{
    ray r(point3(2, 1, 3), vec3(1, 0, 3));
    // point3 hi = r.orig; "is inaccessible" error, private value, handled only inside the class

    point3 origin = r.origin(); // get the origin point
    vec3 direction = r.direction();

    std::cout << "our current ray starts at " << origin << " and points to " << direction << std::endl;

    point3 point_along_ray = r.at(2.0);

    std::cout << "where we are at 2.0 is " << point_along_ray << std::endl;
}