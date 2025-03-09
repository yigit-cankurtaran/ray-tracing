// we'll use this class for colors, locations, directions, offsets etc.
// gotta watch out because this makes it unsafe, we can add different things to each other

#ifndef VEC3_H // if VEC3_H is not defined
#define VEC3_H // start defining the macro

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

    bool near_zero() const
    {
        // if the vector is close to zero in all dimensions return true
        auto s = 1e-8;

        return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
    }

    static vec3 random()
    {
        return vec3(random_double(), random_double(), random_double());
    }
    static vec3 random(double min, double max)
    {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

}; // don't forget the semicolon!

using point3 = vec3;
// point3 is an alias for vec3

// vector utility functions
inline std::ostream &operator<<(std::ostream &out, const vec3 &v)
// inline is a compiler "suggestion" that will replace the function call with actual function code at each call site
{
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3 &v)
{
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline vec3 operator*(const vec3 &v, double t)
{
    return t * v;
    // the function above declares what happens when we multiply a scalar and a vector
    // this function handles the case when we call vector first scalar second
}

inline vec3 operator/(const vec3 &v, double t)
{
    return (1 / t) * v;
}

inline double dot(const vec3 &u, const vec3 &v) // dot product
{
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
    // calculate a scalar from 2 vectors by multiplying their corresponding components
}

// cross product (a new vector perpendicular to both input vectors)
inline vec3 cross(const vec3 &u, const vec3 &v)
{
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
    // first element has 2nd and 3rd elements, 2nd has 1st and 3rd, 3rd has 1st and second elements
}

inline vec3 unit_vector(const vec3 &v)
{
    return v / v.length();
}

// we need this for diffuse (matte) reflection
// when rays hit a matte surface they bounce off in random directions
inline vec3 random_unit_vector()
{
    while (true)
    {
        auto p = vec3::random(-1, 1);    // random vector with components between -1 and 1
        auto lensq = p.length_squared(); // squared length of p
        // if vector is inside sphere (but not too close to zero)
        if (1e-160 < lensq && lensq <= 1)
            // normalize vector to make it exactly length 1
            return p / sqrt(lensq);
    }
}

// dot product of the surface normal and the random vector for hemisphere detection
// if the dot product is positive the vector is correct, if not we need to invert it
// we only want diffuse reflection on outside surfaces
inline vec3 random_on_hemisphere(const vec3 &normal)
{
    vec3 on_unit_sphere = random_unit_vector();
    // checking if it points in the same direction, dot product is about alignment
    if (dot(on_unit_sphere, normal) > 0.0)
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

inline vec3 reflect(const vec3 &v, const vec3 &n)
{
    return v - 2 * dot(v, n) * n;
}

// uv = incoming unit vector (direction of the ray), n = normal vector (where refraction occurs)
// etai_over_etat = ratio of the indices of refraction of the two media the ray is in
inline vec3 refract(const vec3 &uv, const vec3 &n, double etai_over_etat)
{
    // cosine of the incoming vector and the surface normal
    auto cos_theta = std::fmin(dot(-uv, n), 1.0);

    // perpendicular component of the refracted ray (part of the ray bent by the surface)
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    // parallel component of the refracted ray (part of the ray that remains parallel to the surface)
    vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
    // sum of perpendicular and parallel components, final direction of the refracted ray
    return r_out_perp + r_out_parallel;
}

#endif // end the ifndef block