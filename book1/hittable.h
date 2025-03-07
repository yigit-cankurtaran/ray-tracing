#ifndef HITTABLE_H
#define HITTABLE_H

class material;

// storing information about hits
class hit_record
{
public:
    point3 p;
    vec3 normal;
    shared_ptr<material> mat;
    double t;
    bool front_face;

    // hit record normal vector, takes in a ray and a unit length vec3
    void set_face_normal(const ray &r, const vec3 &outward_normal)
    {
        // if the ray and the outward normal are pointing in opposite directions
        // dot < 0 the ray hits the front face, dot >= 0 the ray is hitting the inside
        front_face = dot(r.direction(), outward_normal) < 0;

        // if we're hitting the front face keep the normal pointing outward
        // if we're hitting the back face (inside) flip the normal to point out
        normal = front_face ? outward_normal : -outward_normal;
    }
};

// abstract class so we can't create hittable objects directly. hittable objects need to inherit this.
class hittable
{
public:
    // virtual destructor that uses the default implementation
    virtual ~hittable() = default;

    // ray r input, ray_t is part of the ray to check for hits, hit_record to fill with info if hit occurs
    virtual bool hit(const ray &r, interval ray_t, hit_record &rect) const = 0;
    // pure virtual function, no implementation in this class, functions that inherit must implement.
};

#endif