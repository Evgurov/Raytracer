#ifndef RAY_H
#define RAY_H

#include "geometry.h"


class Ray {
    vec3f direction;
    vec3f starting_point;
    vec3f colour;
    float cur_refractive_index;
public:
    Ray(const vec3f& in_direction, const vec3f& in_starting_point, float refractive_index);
    vec3f& GetDirection() { return direction; };
    vec3f& GetStartingPoint() { return starting_point; };
    vec3f& GetColour() { return colour; };
    Ray Reflect(const vec3f& hit_point, const vec3f& surf_normal, float fuzzieness);
    Ray Refract(const vec3f& hit_point, const vec3f& surf_normal, float new_refractive_index);
    Ray Diffuse(const vec3f& hit_point, const vec3f& surf_normal, float absorbation);
};

#endif