#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : public hittable
{
public:
    // point3 for sphere center, double for sphere radius
    sphere(const point3 &center, double radius) : center(center), radius(std::fmax(0, radius)) {}
    // fmax (float max) ensures radius can never be negative, takes the maximum of 0 and entered radius

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override
    {
        // vector from the ray's origin to the sphere's center
        vec3 oc = center - r.origin();
        // dot product of the ray's direction
        // a vector dotted with itself is the squared length of that vector
        auto a = r.direction().length_squared();
        // dot product of the ray direction and oc
        // tells us how aligned the ray is with the vector to the sphere
        auto h = dot(r.direction(), oc);
        // dot(oc,oc) = squared distance from the ray origin to the sphere center
        // subtracting radius*radius gives us squared distance minus sphere squared radius
        auto c = oc.length_squared() - radius * radius;

        // discriminant > 0 = ray enters and exits
        // discriminant = 0 = ray touches sphere exactly once
        // discriminant < 0 = ray misses sphere completely
        auto discriminant = h * h - a * c;
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        // find nearest root in the acceptable range

        // root according to quadratic formula, h+-sqrtd / a, h instead of b/2 and a instead of 2a
        auto root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root))
        // if root is outside our limits check again
        {
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root))
                // if neither root is in limits we don't have a hit / acceptable hit
                return false;
        }

        // update hit record

        // hit point
        rec.t = root;
        // the hit point on the sphere's surface
        rec.p = r.at(rec.t);
        // vector from the sphere center to the hit point (/ radius to normalize)
        vec3 outward_normal = (rec.p - center) / radius;
        // the ray, the outward normal (unit length)
        rec.set_face_normal(r, outward_normal);

        // we have a hit
        return true;
    }

private:
    // private vars for encapsulation
    // we can't modify these directly but we can create a sphere with these
    point3 center;
    double radius;
};

#endif