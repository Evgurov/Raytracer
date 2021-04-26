#include "ray.h"
#include "geometry.h"

unsigned Ray::max_recursion_depth = 5;

Ray::Ray(const vec3f& in_direction, const vec3f& in_starting_point, float refractive_index, unsigned recursion_depth) {
    direction = in_direction;
    direction = direction.normalize();
    starting_point = in_starting_point;
    cur_refractive_index = refractive_index;
    current_recursion_depth = recursion_depth;
}

Ray Ray::Reflect(const vec3f& hitpoint, const vec3f& normal) const{
    float eps = 1e-3;
    vec3f new_direction(hitpoint + direction - ((normal * (normal * direction)) * 2) - hitpoint);
    Ray reflected_ray(new_direction, hitpoint + (new_direction * eps), cur_refractive_index, current_recursion_depth + 1);
    return reflected_ray;
}

Ray Ray::Refract(const vec3f& hitpoint, const vec3f& normal, float new_refractive_index) const { 
    float min_norm = 1e-5;
    float eps = 1e-3;
    float alpha = acos((-direction) * normal);
    float beta = asin(cur_refractive_index / new_refractive_index * sin(alpha));
    vec3f tang = direction + normal * ((-direction) * normal);
    if (tang.norm() > min_norm){
        tang = tang.normalize();
        vec3f new_direction = (hitpoint - (normal * cos(beta)) + (tang * sin(beta))) - hitpoint;
        Ray refracted_ray(new_direction, hitpoint + (new_direction * eps), new_refractive_index, current_recursion_depth + 1);
        return refracted_ray;
    }
    Ray refracted_ray(direction, hitpoint + (direction * eps), new_refractive_index, current_recursion_depth + 1);
    return refracted_ray;
}

Ray Ray::Diffuse(const vec3f& hitpoint, const vec3f& normal) const {
    float eps = 1e-4;
    vec3f not_normal = normal;
    if (fabs(not_normal.x) < eps) {
        not_normal.x += 1;
    } else if(fabs(not_normal.y) < eps) {
        not_normal.y += 1;
    } else if(fabs(not_normal.z) < eps) {
        not_normal.z += 1;
    } else {
        not_normal.x += 1;
    }
    vec3f e1 = (not_normal - (hitpoint + (normal * (not_normal * normal)))).normalize();
    vec3f e2 = cross(normal, e1).normalize();

    float phi = float(std::rand());
    phi = phi / RAND_MAX * 2 * PI;

    float teta = float(std::rand());
    teta = teta / RAND_MAX * PI / 2;

    vec3f new_direction = e1 * sin(teta) * cos(phi) + e2 * sin(teta) * sin(phi) + normal * cos(teta);

    Ray diffused_ray(new_direction, hitpoint, cur_refractive_index, current_recursion_depth + 1);

    return diffused_ray;
}