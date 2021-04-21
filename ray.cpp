#include "ray.h"
#include "geometry.h"

Ray::Ray(const vec3f& in_direction, const vec3f& in_starting_point, float refractive_index) {
    direction = in_direction;
    direction = direction.normalize();
    starting_point = in_starting_point;
    cur_refractive_index = refractive_index;
}
