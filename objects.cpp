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
            if ((hitpoint - ray.GetStartingPoint()).norm() < (min_hitpoint - ray.GetStartingPoint()).norm()) {
                min_hitpoint = hitpoint;
                closest_object = i;
            }
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

Cilinder::Cilinder(Material* in_material, vec3f& in_center, float in_radius, float in_height) : Object(in_material) {
    center = in_center; 
    radius = in_radius;
    height = in_height;
}

bool Cilinder::Hitted(Ray& ray, vec3f& hitpoint) const {
    vec3f startpoint = ray.GetStartingPoint();
    vec3f direction = ray.GetDirection();
    float A = direction.x * direction.x + direction.y * direction.y;
    float B = 2 * startpoint.x * direction.x - 2 * startpoint.x * center.x + 2 * startpoint.y * direction.y - 2 * direction.y * center.y;
    float C = startpoint.x * startpoint.x + center.x * center.x - 2 * startpoint.x * center.x + startpoint.y * startpoint.y + center.y * center.y - 2 * startpoint.y * center.y - radius * radius;
    float D = B*B - 4*A*C;
    if (D < 0) {
        return false;
    } else {
        float t1 = (-B - sqrtf(D)) / (2 * A);
        float t2 = (-B + sqrtf(D)) / (2 * A);
        if (t1 < 0) {
            if (t2 < 0)
                return false;
            else {
                vec3f hit2 = startpoint + (direction * t2);
                if (startpoint.z > center.z + height/2) {
                    if (hit2.z > center.z + height/2){
                        return false;
                    } else {
                        float t3 = (center.z + height/2 - startpoint.z) / direction.z;
                        hitpoint = startpoint + (direction * t3);
                    }
                } else if (fabs(startpoint.z - center.z) < height/2){
                    if (hit2.z > center.z + height/2){
                        float t3 = (center.z + height/2 - startpoint.z) / direction.z;
                        hitpoint = startpoint + (direction * t3);
                    } else if (fabs(hit2.z - center.z) < height/2){
                        hitpoint = hit2;
                    } else {
                        float t3 = (center.z - height/2 - startpoint.z) / direction.z;
                        hitpoint = startpoint + (direction * t3);
                    }
                } else {
                    if (hit2.z < center.z - height/2){
                        return false;
                    } else {
                        float t3 = (center.z - height/2 - startpoint.z) / direction.z;
                        hitpoint = startpoint + (direction * t3);
                    }
                }
            }
        }   
        else {
            vec3f hit1 = startpoint + (direction * t1);
            vec3f hit2 = startpoint + (direction * t2);
            if (fabs(hit1.z - center.z) < height/2){
                hitpoint = hit1;
                return true;
            } else if(hit1.z > center.z + height/2){
                if (hit2.z > center.z + height/2){
                    return false;
                } else {
                    float t3 = (center.z + height/2 - hit1.z) / direction.z;
                    hit1 = hit1 + (direction * t3);
                    hitpoint = hit1;
                }
            } else {
                if (hit2.z < center.z - height/2){
                    return false;
                } else {
                    float t3 = (center.z - height/2 - hit1.z) / direction.z;
                    hit1 = hit1 + (direction * t3);
                    hitpoint = hit1;
                }
            }
        }
    }
    return true;
}