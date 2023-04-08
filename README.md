# Model Ray tracing renderer (C++, CPU)
## Dependencies
The project was developed and ran on `Linux`.
To work and build, the `OpenGL` library `glfw3` is required. You can install this by typing in the command line
```
$ sudo apt-get install libglfw3-dev
```
To build the program, the `g++` compiler and the `cmake` and `make` utilities are also required.

## Build and launch
In the project folder:
```
$ cmake ./
$ make
$ ./bin/main
```
The project goals did not include the user interface for creating a scene (but can be considered as its further development), so the result of the program is one - demonstration of the capabilities of the "engine": after rendering, a render of a predefined scene will appear in a separate window. Rendering performs on the `cpu` and takes a significant amount of time, so by default a stripped-down image is generated. There are several heavier pre-rendered images in the `./resources` folder.

## Briefly about the work done
In this project, I completely wrote a small engine for ray tracing in C++ (for educational purposes, pure C++ without STL). The only ready-made code used is pixel-by-pixel rendering of the window in OpenGL.

Specifically implemented:

- `geometry.h`: template library of n-dimensional vectors of arbitrary type with all basic operators and special vector operations

- the `camera` module:
  * camera object with all the necessary settings:
    - location and orientation in space
    - viewing angle fov
    - resolution of the camera matrix
    - anti-aliasing level (rays per pixel)
- the `objects` module:
  * Hierarchy of materials and graphic objects with their own methods of interaction with the intersected beam and intersection search algorithm.
  * Materials:
    - Emissive
    - Dielectric
    - Diffuse
  * Objects:
    - Polygon
    - Polygonal object(collection of plygons)
    - Sphere
    - Cilinder
  * Class `scene` (collection of objects)

- module `interactonModels`:
- hierarchy of materials (models of interaction with the object). Interaction models are implemented:
- `Schlick`: interaction with a translucent object taking into account:
- refractive coefficients (according to Snellius' law)
- the spectrum of internal light absorption (in the thickness of the material)
- the Fresnel factor (by means of the Schlick approximation)
- the effect of total internal reflection
- `Lambert`: interaction with the absolute Lambert surface, taking into account
- angle of incidence on the material and total scattering (Lambert model)
- the spectrum of the surface half-absorption of light
- `SimpleEmission`: interaction with a simple radiating material (the most elementary model)

- `main.cpp `: setting the scene and rendering using the modules listed above

Also:
- the recursion depth and the accuracy of uniform scattering are adjusted
- to check the intersection with polygons, a fast Meller-Trambor algorithm is implemented

---
Gurov Evgeny 2021.
