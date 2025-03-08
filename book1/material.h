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

// clear materials such as water, glass, diamond etc.
// when rays hit them, it splits into a reflected and a refracted (transmitted) ray
class dielectric : public material
{
public:
    dielectric(double refraction_index) : refraction_index(refraction_index) {}

    bool scatter(const ray &r_in, const hit_record &rec, color &attenuation, ray &scattered) const override
    {
        attenuation = color(1.0, 1.0, 1.0);

        // if front face this needs to be divided bc it's crossing over to the dielectric
        // if back face this is the refraction index bc it's crossing over to the surrounding materials
        double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;

        vec3 unit_direction = unit_vector(r_in.direction());

        // theta is the angle the ray hits the material with

        double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

        // if we hit at a too large angle we reflect, don't refract
        bool cannot_refract = ri * sin_theta > 1.0;
        vec3 direction;

        if (cannot_refract)
            direction = reflect(unit_direction, rec.normal);
        else
            direction = refract(unit_direction, rec.normal, ri);

        scattered = ray(rec.p, direction);
        return true;
    }

private:
    // refractive index in vacuum or air, or the ratio of the two media the ray goes through
    double refraction_index;
};

#endif