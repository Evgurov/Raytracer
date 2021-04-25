#include <cmath>
#include "camera.h"
#include "geometry.h"
#include "Image.h"
#include "ray.h"
#include "objects.h"

vec3f get_ray_colour(Ray& ray);

vec3f getPixelColour(Camera& camera, unsigned x, unsigned y);

Camera::Camera(vec3f& location_vec, vec3f& view_vec, vec2f& in_phisical_screensize, vec2u& in_pixel_screensize, float input_fov) {
    vec3f world_up_vec(0.0f, 0.0f, 1.0f);
    right = cross(view_vec, world_up_vec).normalize();
    up = cross(right, view_vec).normalize();
    front = view_vec.normalize();

    location = location_vec;
    
    phisical_screensize = in_phisical_screensize;

    pixel_screensize = in_pixel_screensize;

    fov = input_fov;

    screen_dist = phisical_screensize.x / tan((PI - fov)/2);

    leftdown_screen_angle = location + (front * screen_dist) - (right * phisical_screensize.x * 0.5) - (up * phisical_screensize.y * 0.5);

    pixel_size = phisical_screensize.x / pixel_screensize.x;
}

Ray Camera::Gen_ray(unsigned x, unsigned y) {
    vec3f pixel_coords(leftdown_screen_angle + (up * pixel_size * y) + (right * pixel_size * x));
    Ray ray(pixel_coords - location, location, 1.0f, 0);
    return ray;
}

void Camera::Render(Image& screenBuffer, Scene& scene) { 
    Pixel pixel;
    vec3f colour;
    for (unsigned i = 0; i < pixel_screensize.y; i++) {
        for (unsigned j  = 0; j < pixel_screensize.x; j++) {
            if (i == 512 && j == 512)
                colour = colour * 2;
            Ray origin_ray = Gen_ray(j, i);
            colour = scene.Intersect(origin_ray);
            pixel.r = int(255.99 * colour.x);
            pixel.g = int(255.99 * colour.y);
            pixel.b = int(255.99 * colour.z); 
            pixel.a = 255;
            screenBuffer.PutPixel(j, i, pixel);
        }
    }
}
