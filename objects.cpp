#include <cmath>

#include "objects.h"
#include "geometry.h"
#include "ray.h"


vec3f DielectricMaterial::GetRayColour(const Ray& ray, const vec3f& hitpoint, const vec3f& normal, const Side& side, const Scene& scene) const {
    vec3f max_recursion_colour(0.0f, 0.0f, 0.0f);
    float R, R0, Teta;
    if (side == OUTSIDE) {
        R0 = (outer_refractive_index - inner_refractive_index) / (outer_refractive_index + inner_refractive_index);
        R0 *= R0;
    } else {
        R0 = (inner_refractive_index - outer_refractive_index) / (outer_refractive_index + inner_refractive_index);
        R0 *= R0;
    }
    Teta = (-ray.GetDirection()) * normal;
    R = R0 + (1 - R0) * pow(1 - Teta, 5);
    if (ray.GetCurRecursionDepth() < ray.GetMaxRecursionDepth()){
        if (side == OUTSIDE) {
            return ((scene.Intersect(ray.Reflect(hitpoint, normal)) * R) + (scene.Intersect(ray.Refract(hitpoint, normal, inner_refractive_index)) * (1 - R)));
        } else {
            return ((scene.Intersect(ray.Reflect(hitpoint, normal)) * R) + (scene.Intersect(ray.Refract(hitpoint, normal, outer_refractive_index)) * (1 - R)));   
        }
    } else {
        return max_recursion_colour;
    }
}


vec3f Scene::Intersect(const Ray& ray) const {
    vec3f background_colour(0.6f, 0.8f, 1.0f);
//    vec3f background_colour(0.0f, 0.0f, 0.0f);
    vec3f hitpoint;
    vec3f normal;
    Side side;
    vec3f min_hitpoint;
    vec3f min_normal;
    Side min_side;
    vec3f infinity(1000.0f, 1000.0f, 1000.0f);
    int closest_object = -1;
    min_hitpoint = infinity;
    for (int i = 0; i < objects.size(); i++){
        if(objects[i] -> Hitted(ray, hitpoint, normal, side)) {
            if ((hitpoint - ray.GetStartingPoint()).norm() < (min_hitpoint - ray.GetStartingPoint()).norm()) {
                min_hitpoint = hitpoint;
                min_normal = normal;
                min_side = side;
                closest_object = i;
            }
        }
    }
    if (closest_object == -1)
        return background_colour;
    else {
        return objects[closest_object] -> GetRayColour(ray, min_hitpoint, min_normal, min_side, *this);
    }
}

bool Sphere::Hitted(const Ray& ray, vec3f& hitpoint, vec3f& normal, Side& side) const {
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
        else {
            hitpoint = ray.GetStartingPoint() + (ray.GetDirection() * t2);
            normal = (center - hitpoint).normalize();
            side = INSIDE;
        } 

    else {
        hitpoint = ray.GetStartingPoint() + (ray.GetDirection() * t1);
        normal = (hitpoint - center).normalize();
        side = OUTSIDE;
    }
    return true;
}

Cilinder::Cilinder(Material* in_material, vec3f& in_center, float in_radius, float in_height) : Object(in_material) {
    center = in_center; 
    radius = in_radius;
    height = in_height;
}

bool Cilinder::Hitted(const Ray& ray, vec3f& hitpoint, vec3f& normal, Side& side) const {
    vec3f up(0.0f, 0.0f, 1.0f);
    vec3f down(0.0f, 0.0f, -1.0f);
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
                        normal = up;
                        side = OUTSIDE;
                    }
                } else if (fabs(startpoint.z - center.z) < height/2){
                    if (hit2.z > center.z + height/2){
                        float t3 = (center.z + height/2 - startpoint.z) / direction.z;
                        hitpoint = startpoint + (direction * t3);
                        normal = down;
                        side = INSIDE;
                    } else if (fabs(hit2.z - center.z) < height/2){
                        hitpoint = hit2;
                        normal = up * ((hitpoint - center) * up) - hitpoint;
                        side = INSIDE;
                    } else {
                        float t3 = (center.z - height/2 - startpoint.z) / direction.z;
                        hitpoint = startpoint + (direction * t3);
                        normal = up;
                        side = INSIDE;
                    }
                } else {
                    if (hit2.z < center.z - height/2){
                        return false;
                    } else {
                        float t3 = (center.z - height/2 - startpoint.z) / direction.z;
                        hitpoint = startpoint + (direction * t3);
                        normal = down;
                        side = OUTSIDE;
                    }
                }
            }
        }
        else {
            vec3f hit1 = startpoint + (direction * t1);
            vec3f hit2 = startpoint + (direction * t2);
            if (fabs(hit1.z - center.z) < height/2){
                hitpoint = hit1;
                normal = hitpoint - up * ((hitpoint - center) * up);
                side = OUTSIDE;
                return true;
            } else if(hit1.z > center.z + height/2){
                if (hit2.z > center.z + height/2){
                    return false;
                } else {
                    float t3 = (center.z + height/2 - hit1.z) / direction.z;
                    hit1 = hit1 + (direction * t3);
                    normal = up;
                    side = OUTSIDE;
                    hitpoint = hit1;
                }
            } else {
                if (hit2.z < center.z - height/2){
                    return false;
                } else {
                    float t3 = (center.z - height/2 - hit1.z) / direction.z;
                    hit1 = hit1 + (direction * t3);
                    normal = down;
                    side = OUTSIDE;
                    hitpoint = hit1;
                }
            }
        }
    }
    return true;
}