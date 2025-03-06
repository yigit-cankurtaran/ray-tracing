#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

class hittable_list : public hittable
{
public:
    // can hold any type of object that inherits from hittable
    std::vector<shared_ptr<hittable>> objects;
    // shared_ptr is a smart pointer that:
    // automatically manages memory,
    // has a reference counter, every time they go out of scope they get decremented, once object hits 0 it's safely deleted
    // can be shared by multiple parts of the code,
    // is safer than raw pointers

    // constructors, top one is for the empty and the bottom one is if we pass in something
    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    // empty the objects list
    void clear() { objects.clear(); }

    // add an object to the list
    void add(shared_ptr<hittable> object)
    {
        objects.push_back(object);
    }

    // take a ray and check if it hits any object in our list, keep track of the closest hit (we don't want objects behind other objects visible)
    // update the hit record with info about the closest hit
    bool hit(const ray &r, double ray_tmin, double ray_tmax, hit_record &rec) const override
    {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_tmax;

        for (const auto &object : objects)
        {
            if (object->hit(r, ray_tmin, closest_so_far, temp_rec))
            // -> is used to access members. this is the equivalent to writing (*object).hit
            // we call the hit method on whatever object points to
            {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }
};

#endif