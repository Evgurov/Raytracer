#ifndef OBJECTS_H
#define OBJECTS_H

#include <vector>
#include <string>
#include "geometry.h"
#include "ray.h"

//--------ALL DEFINED CLASSES-------------------------
class Material;
class OrdinaryMaterial;
class Scene;
class Object;
class PolygonalObject;
class GeometricObject;
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
    Material* material;
};

//--------------MATERIALS------------------------------

//_______base material class___________________________

class Material {
public:
    virtual vec3f GetRayColour(Ray& ray) const = 0;
};

class OrdinaryMaterial : public Material {
    vec3f colour;
public:
    OrdinaryMaterial(vec3f& in_colour) { colour = in_colour; };
    vec3f GetRayColour(Ray& ray) const { return colour; };
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
public:
    virtual bool Hitted(Ray& ray, vec3f& hitpoint) const = 0;
    virtual vec3f GetRayColour(Ray& ray) const = 0;
};

//________polygonal object class_________________________

class PolygonalObject : public Object{
    std::vector<Vertex> vertexes;
    std::vector<Polygon> polygons;
public:
    PolygonalObject(Material& in_material, const std::string& objfile_path);
    bool Hitted(Ray& ray, vec3f& hitpoint) const;
    virtual vec3f GetRayColour(Ray& ray) const;
    ~PolygonalObject();
};

//________geometric object class_________________________
class GeometricObject : public Object{
    Material* material;
public:
    GeometricObject(Material* in_material) { material = in_material; };
    Material* GetMaterial() const { return material; };
    virtual bool Hitted(Ray& ray, vec3f& hitpoint) const = 0;
    virtual vec3f GetRayColour(Ray& ray) const = 0;
};

//________class for spheres_______________________________
class Sphere : public GeometricObject {
    vec3f center;
    float radius;
public:
    Sphere(Material* in_material, vec3f& in_center, float in_radius) : GeometricObject(in_material) { center = in_center; radius = in_radius; };
    vec3f GetCenter() const { return center; };
    float GetRadius() const { return radius; };
    bool Hitted(Ray& ray, vec3f& hitpoint) const;
    vec3f GetRayColour(Ray& ray) const;
};

//--------------------------------------------------------
#endif