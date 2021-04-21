#include <cmath>

#include "objects.h"
#include "geometry.h"
#include "ray.h"

vec3f Scene::Intersect(Ray& ray) {
    vec3f background_colour(1.0f, 1.0f, 1.0f);
    vec3f hitpoint;
    vec3f min_hitpoint;
    vec3f infinity(1000.0f, 1000.0f, 1000.0f);
    int closest_object = -1;
    min_hitpoint = infinity;
    for (int i = 0; i < objects.size(); i++){
        if(objects[i] -> Hitted(ray, hitpoint)) {
            if ((hitpoint - ray.GetStartingPoint()).norm() < (min_hitpoint - ray.GetStartingPoint()).norm())
                min_hitpoint = hitpoint;
                closest_object = i;
        }
    }
    if (closest_object == -1)
        return background_colour;
    else {
        return objects[closest_object] -> GetRayColour(ray);
    }
}

bool Sphere::Hitted(Ray& ray, vec3f& hitpoint) const {
    vec3f center_radiusvec = center - ray.GetStartingPoint();
    float projection_length = ray.GetDirection() * center_radiusvec;
    float dist2 = center_radiusvec * center_radiusvec - projection_length * projection_length;
    if (dist2 > radius * radius)
        return false;
    float offset = sqrt(radius * radius - dist2);
    float t1 = projection_length - offset;
    float t2 = projection_length + offset;
    if (t1 < 0)
        if (t2 < 0)
            return false;
        else 
            hitpoint = ray.GetStartingPoint() + ray.GetDirection() * t2;
    else 
        hitpoint = ray.GetStartingPoint() + ray.GetDirection() * t1;
    return true;
}

vec3f Sphere::GetRayColour(Ray& ray) const{
    return GetMaterial() -> GetRayColour(ray);
}
