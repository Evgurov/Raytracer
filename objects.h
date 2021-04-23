#ifndef OBJECTS_H
#define OBJECTS_H

#include <vector>
#include <string>
#include "geometry.h"
#include "ray.h"

//--------ALL DEFINED CLASSES-------------------------
class Material;
class OrdinaryMaterial;
class DiffuseMaterial;
class Scene;
class Object;
class PolygonalObject;
class Sphere;
//----------------------------------------------------

//_______struct vertex for polygonal objects__________
struct Vertex{
    float x;
    float y;
    float z;
};

//_______struct polygon for polygonal objects_________
struct Polygon{
    unsigned first_vertex;
    unsigned second_vertex;
    unsigned third_vertex;
    vec3f normal;
};

//--------------MATERIALS------------------------------

//_______base material class___________________________

class Material {
public:
    virtual vec3f GetRayColour(Ray& ray) const = 0;
};

class EmissiveMaterial : public Material {
    vec3f colour;
public:
    EmissiveMaterial(vec3f& in_colour) { colour = in_colour; };
    vec3f GetRayColour(Ray& ray) const { return colour; };
};

class DiffuseMaterial : public Material {
    vec3f colour;
    float fuzzieness;
    float albedo;
public:
    DiffuseMaterial(vec3f& in_colour, float in_fuzzieness, float in_albedo) : colour(in_colour), fuzzieness(in_fuzzieness), albedo(in_albedo) {};
    vec3f GetColour() const { return colour; };
    float GetFuzzieness() const { return fuzzieness; };
    float GetAlbedo() const { return albedo; };
    vec3f GetRayColour(Ray& ray) const ;
};

//------------------------------------------------------

//_______class Scene for storing graphic objects________

class Scene {
    std::vector<Object*> objects;
public:
    void AddObject(Object* new_object) { objects.push_back(new_object); }
    vec3f Intersect (Ray& ray);
};

//-------OBJECTS-----------------------------------------

//_______base object class_______________________________

class Object {
    Material* material;
public:
    Object(Material* in_material) { material = in_material; };
    virtual bool Hitted(Ray& ray, vec3f& hitpoint) const = 0;
    virtual vec3f GetRayColour(Ray& ray) const { return material -> GetRayColour(ray); }
    Material* GetMaterial() const { return material; };
};

//________polygonal object class_________________________

class PolygonalObject : public Object{
    std::vector<Vertex> vertexes;
    std::vector<Polygon> polygons;
public:
    PolygonalObject(Material* in_material, const std::string& objfile_path);
    bool Hitted(Ray& ray, vec3f& hitpoint) const;
    virtual vec3f GetRayColour(Ray& ray) const;
    ~PolygonalObject();
};

//________class for spheres_______________________________

class Sphere : public Object {
    vec3f center;
    float radius;
public:
    Sphere(Material* in_material, vec3f& in_center, float in_radius) : Object(in_material), center(in_center), radius(in_radius) {};
    vec3f GetCenter() const { return center; };
    float GetRadius() const { return radius; };
    bool Hitted(Ray& ray, vec3f& hitpoint) const;
};

//________class for Cilinders_________________________________

class Cilinder : public Object {
    vec3f center;
    float radius;
    float height;
public:
    Cilinder(Material* in_material, vec3f& in_center, float in_radius, float in_height);
    vec3f GetCenter() const { return center; };
    float GetRadius() const { return radius; };
    float GetHeight() const { return height; };
    bool Hitted(Ray& ray, vec3f& hitpoint) const;
};

//--------------------------------------------------------
#endif