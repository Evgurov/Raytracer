#ifndef RAY_H
#define RAY_H

#include "geometry.h"


class Ray {
    vec3f direction;
    vec3f starting_point;
    float cur_refractive_index;
    unsigned current_recursion_depth;
    static unsigned max_recursion_depth;
public:
    Ray(const vec3f& in_direction, const vec3f& in_starting_point, float refractive_index, unsigned recursion_depth);
    vec3f GetDirection() const { return direction; };
    vec3f GetStartingPoint() const { return starting_point; };
    float GetRefrectiveIndex() const { return cur_refractive_index; };
    unsigned GetCurRecursionDepth() const { return current_recursion_depth; };
    unsigned GetMaxRecursionDepth() const { return max_recursion_depth; };
    Ray Reflect(const vec3f& hit_point, const vec3f& normal) const;                             //casual relection
    Ray Refract(const vec3f& hit_point, const vec3f& normal, float new_refractive_index) const; //snell's law
    Ray Diffuse(const vec3f& hit_point, const vec3f& normal) const;
};

#endif