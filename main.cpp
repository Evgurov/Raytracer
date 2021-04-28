#include "common.h"
#include "Image.h"
#include <GLFW/glfw3.h>
#include "geometry.h"
#include "camera.h"
#include "ray.h"
#include "objects.h"


//constexpr GLsizei WINDOW_WIDTH = 512, WINDOW_HEIGHT = 512;

void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mode) {
    switch (key)
    {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        default:
            break;
    }
}

int initGL() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
	    std::cout << "Failed to initialize OpenGL context" << std::endl;
	    return -1;
    }
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    return 0;
}


int main(int argc, char** argv) {

    unsigned WINDOW_WIDTH, WINDOW_HEIGHT;

    std::string config;

    std::cout << "Enter '-w 1024' to swap to 1024x1024 resolution." << std::endl;
    std::cout << "To left with 512x512 enter anyting else." << std::endl;

    std::cin >> config;

    if (config == "-w 1024"){
        WINDOW_WIDTH = 1024;
        WINDOW_HEIGHT = 1024;
    } else {
        WINDOW_WIDTH = 512;
        WINDOW_HEIGHT = 512;
    }

    Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4); //buffer to render in
    
    vec3f colour1(0.9f, 0.9f, 0.9f); 
    vec3f colour2(0.5f, 0.5f, 0.8f);
    vec3f colour3(0.4f, 0.4f, 0.4f);

    vec3f absorbation_spectre(0.5f, 0.5f, 0.5f);
    vec3f absorbation_spectre1(0.8f, 0.5f, 0.4f);
    vec3f absorbation_spectre2(0.3f, 0.4f, 0.8f);

    EmissiveMaterial emissive1(colour1);
    EmissiveMaterial emissive2(colour2);
    EmissiveMaterial emissive3(colour3);

    DielectricMaterial glass(1.5f, 1.0f);

    DielectricMaterial diamond(2.4f, 1.5f);

    DiffuseMaterial cement(absorbation_spectre);
    DiffuseMaterial gips(absorbation_spectre1);
    DiffuseMaterial blue_gips(absorbation_spectre2);

//-------sphere creation------------------------------------------------------------

    vec3f light_center(-4.0f, -4.0f, 3.0f);

    vec3f sphere_center(0.0f, 0.0f, 2.0f);

    vec3f glass_sphere_center(1.0f, -4.0f, 2.0f);

    vec3f diffuse_sphere_center(-7.0f, -3.0f, 1.0f);

    Sphere light(&emissive1, light_center, 2.0f);

    Sphere sphere(&diamond, sphere_center, 0.5f);

    Sphere glass_sphere(&glass, glass_sphere_center, 2.0f);

    Sphere diffuse_sphere(&blue_gips, diffuse_sphere_center, 1.5f);

    Sphere sky(&emissive1, sphere_center, 40.0f);

//----------------------------------------------------------------------------------

//-------cilinder creation----------------------------------------------------------

    vec3f pedestal_center(0.0f, 0.0f, -0.5f);
    Cilinder pedestal(&cement, pedestal_center, 2.0f, 1.0f);
    vec3f table_center(0.0f, 0.0f, -1.5f);
    Cilinder table(&gips, table_center, 10.0f, 1.0f);

//----------------------------------------------------------------------------------

//--------octahedron creation-------------------------------------------------------

    std::vector<Polygon> octahedron_polygons;

    vec3f vertex1(0.0f, 0.0f, 0.0f);
    vec3f vertex2(2.0f, 0.0f, 2.0f);
    vec3f vertex3(0.0f, 2.0f, 2.0f);
    vec3f vertex4(-2.0f, 0.0f, 2.0f);
    vec3f vertex5(0.0f, -2.0f, 2.0f);
    vec3f vertex6(0.0f, 0.0f, 4.0f);

    Polygon polygon1(vertex1, vertex3, vertex2);
    octahedron_polygons.push_back(polygon1);

    Polygon polygon2(vertex1, vertex4, vertex3);
    octahedron_polygons.push_back(polygon2);

    Polygon polygon3(vertex1, vertex5, vertex4);
    octahedron_polygons.push_back(polygon3);

    Polygon polygon4(vertex1, vertex2, vertex5);
    octahedron_polygons.push_back(polygon4);

    Polygon polygon5(vertex6, vertex2, vertex3);
    octahedron_polygons.push_back(polygon5);

    Polygon polygon6(vertex6, vertex3, vertex4);
    octahedron_polygons.push_back(polygon6);

    Polygon polygon7(vertex6, vertex4, vertex5);
    octahedron_polygons.push_back(polygon7);

    Polygon polygon8(vertex6, vertex5, vertex2);
    octahedron_polygons.push_back(polygon8);

    PolygonalObject octahedron(&glass, octahedron_polygons);

//---------scene creation-----------------------------------------------------------
    Scene scene;
    scene.AddObject(&light);
    scene.AddObject(&pedestal);
    scene.AddObject(&table);
    scene.AddObject(&sphere);
    scene.AddObject(&glass_sphere);
    scene.AddObject(&octahedron);
    scene.AddObject(&diffuse_sphere);
//    scene.AddObject(&sky);
//----------------------------------------------------------------------------------
    
//---------camera creation----------------------------------------------------------
    vec3f cam_location(2.0f, 6.0f, 5.0f);
    vec3f viewvec = pedestal_center - cam_location;
    vec2f phisical_screensize(2.0f, 2.0f);
    vec2u pixel_screensize(WINDOW_WIDTH, WINDOW_HEIGHT);
    float fov = PI/3;
    Camera camera(cam_location, viewvec, phisical_screensize, pixel_screensize, fov);
//----------------------------------------------------------------------------------
    
    camera.Render(screenBuffer, scene); //rendering


    if(!glfwInit())
        return -1;

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow*  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Witch's ball", nullptr, nullptr);
    if (window == nullptr) {
	    std::cout << "Failed to create GLFW window" << std::endl;
	    glfwTerminate();
	    return -1;
    }
	
    glfwMakeContextCurrent(window); 
    glfwSetKeyCallback(window, keyCallBack);

    if(initGL() != 0) {
	    return -1;
    }

//  Reset any OpenGL errors which could be present for some reason
    GLenum gl_error = glGetError();
    while (gl_error != GL_NO_ERROR)
	    gl_error = glGetError();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glViewport(0, 0, WINDOW_WIDTH , WINDOW_HEIGHT);                                             GL_CHECK_ERRORS;
        glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
        
    	glfwSwapBuffers(window);
    }
    screenBuffer.Save("../resources/night_test_image.png");
    
    glfwTerminate();
    return 0;
}