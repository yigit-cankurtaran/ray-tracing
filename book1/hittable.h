#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

// storing information about hits
class hit_record
{
public:
    point3 p;
    vec3 normal;
    double t;
};

// abstract class so we can't create hittable objects directly. hittable objects need to inherit this.
class hittable
{
public:
    // virtual destructor that uses the default implementation
    virtual ~hittable() = default;

    // ray r input, ray_tmin and ray_tmax are how far along the ray to check for hits, hit_record to fill with info if hit occurs
    virtual bool hit(const ray &r, double ray_tmin, double ray_tmax, hit_record &rect) const = 0;
    // pure virtual function, no implementation in this class, functions that inherit must implement.
};

#endif