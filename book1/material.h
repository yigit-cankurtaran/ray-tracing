#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material
{
public:
    virtual ~material() = default;

    virtual bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const
    {
        return false;
    }
};

// we'll create albedo and have it always scatter instead of getting reabsorbed

class lambertian : public material
{
public:
    lambertian(const color &albedo) : albedo(albedo) {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered)
        const override
    {
        auto scatter_direction = rec.normal + random_unit_vector();

        // catch degenerate scatter direction
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

private:
    color albedo;
};

class metal : public material
{
public:
    metal(const color &albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
    {
        // reflection of the incoming ray's direction off the surface normal
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        // add the fuzziness factor to the reflection, simulating a rough surface
        reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
        // create a new ray that originates from the hit point in the direction of reflected
        scattered = ray(rec.p, reflected);
        // color reflection is the albedo
        attenuation = albedo;
        // return true if the ray is scattered away from the surface, otherwise false (ray's absorbed)
        return (dot(scattered.direction(), rec.normal) > 0);
    }

private:
    color albedo; // reflection
    double fuzz;  // fuzziness factor, kinda like distortion
};

#endif