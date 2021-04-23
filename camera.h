#ifndef CAMERA_H
#define CAMERA_H

#include "geometry.h"
#include "ray.h"
#include "Image.h"
#include "objects.h"

constexpr float PI = 3.1415;

class Camera {
    vec3f location;
    vec3f up;
    vec3f right;
    vec3f front;
    vec2f phisical_screensize;
    vec2u pixel_screensize;
    float fov;
    float screen_dist;
    vec3f leftdown_screen_angle;
    float pixel_size;
public:
    Camera (vec3f& location_vec, vec3f& view_vec, vec2f& phisical_screensize, vec2u& screensize, float input_fov);
    Ray Gen_ray(unsigned x, unsigned y);            //generates origin ray
    void Render(Image& screenBuffer, Scene& scene); //renders the image of the scene to buffer
};

#endif