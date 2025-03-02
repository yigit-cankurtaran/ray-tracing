// we'll use this class for colors, locations, directions, offsets etc.
// gotta watch out because this makes it unsafe, we can add different things to each other

#ifndef VEC3_H // if VEC3_H is not defined
#define VEC3_H // start defining the macro

#include <cmath> // math.h from C
#include <iostream>

class vec3
{
public:
    double e[3];

    vec3() : e{0, 0, 0} {} // constructor for vec3
    // e is initialized to 0, the {} is the empty constructor body

    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const { return e[0]; }
    // x function that returns a double. const = function doesn't modify object
    // readability and logical concerns. v.x() is more understandable than v.e[0]
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    // unary operator -, returns a new vec3 with all components negated
    double operator[](int i) const { return e[i]; }
    // const version of operator[], returns the i-th component of the vector
    // e.g. double y_value = v[1]
    double &operator[](int i) { return e[i]; }
    // non-const version of operator[], returns a reference to the i-th component of the vector
    // e.g. v[1] = 5
    // we need both bc if we only have a const vector we can't modify it and if we have non-const we can modify that one.

    vec3 &operator+=(const vec3 &v)
    // we want vec3& because we don't want new objects
    // and it allows to chain operations
    {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
        // this is a special pointer for every non-static member function
        // this is a pointer to the current object, * dereferences it
    }

    vec3 &operator*=(double t)
    {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3 &operator/=(double t)
    {
        return *this *= 1 / t;
    }

    double length() const
    {
        return std::sqrt(length_squared());
    }
    double length_squared() const
    {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }
    // we have both because for comparisons we can use length_squared
    // sqrt is computationally expensive

}; // don't forget the semicolon!

#endif // end the ifndef block