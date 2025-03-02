#ifndef RAY_H
#define RAY_H

#include "vec.h"

class ray
{
public:
    ray() {} // constructor for no inputs
    ray(const point3 &origin, const vec3 &direction) : orig(origin), dir(direction) {}
    // initializing members, orig takes the value of origin and dir takes the value of direction
    /* this is pretty much the following,
    ray(const point3 &origin, const vec3 &direction) {
    orig = origin;
    dir = direction;
}
but initializing them in the constructor is far more efficient
    */

    // getter methods
    const point3 &origin() const { return orig; }
    const vec3 &direction() const { return dir; }

    point3 at(double t) const
    {
        return orig + t * dir;
        // the ray formula
        // starts at the origin, t long in the direction of dir
        // we use orig and dir because we don't want outside effects
    }

private:
    point3 orig;
    vec3 dir;
    // member variables, belong to a class and store state or attributes
    // they exist as long as the object exists
    // private because we don't want outside access to these, access with getters and setters
    // orig and dir are handled inside the class, origin and direction can be accessed by outside
};

#endif