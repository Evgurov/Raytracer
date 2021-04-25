#include "common.h"
#include "Image.h"
#include <GLFW/glfw3.h>
#include "geometry.h"
#include "camera.h"
#include "ray.h"
#include "objects.h"


constexpr GLsizei WINDOW_WIDTH = 1024, WINDOW_HEIGHT = 1024; 

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


    Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4); //buffer to render in
    
    vec3f colour1(0.9f, 0.9f, 0.9f); 
    vec3f colour2(0.5f, 0.5f, 0.8f);
    vec3f colour3(0.4f, 0.4f, 0.4f);

    EmissiveMaterial emissive1(colour1);
    EmissiveMaterial emissive2(colour2);
    EmissiveMaterial emissive3(colour3);
    DielectricMaterial glass(1.5f, 1.0f);

//-------sphere creation------------------------------------------------------------

    vec3f sphere_center(0.0f, 0.0f, 0.0f);

    vec3f sphere_center1(2.5f, -4.0f, 0.0f);

    Sphere sphere(&glass, sphere_center, 2.0f);

    Sphere sphere1(&emissive1, sphere_center1, 1.0f);

//----------------------------------------------------------------------------------

//-------cilinder creation----------------------------------------------------------

    vec3f pedestal_center(0.0f, 0.0f, -2.5f);
    Cilinder pedestal(&emissive2, pedestal_center, 2.0f, 1.0f);
    vec3f table_center(0.0f, 0.0f, -3.0f);
    Cilinder table(&emissive3, table_center, 10.0f, 1.0f);

//---------scene creation-----------------------------------------------------------
    Scene scene;
    scene.AddObject(&sphere);
//    scene.AddObject(&pedestal);
    scene.AddObject(&table);
    scene.AddObject(&sphere1);
//----------------------------------------------------------------------------------
    
//---------camera creation----------------------------------------------------------
    vec3f cam_location(0.0f, 5.0f, 2.0f);
    vec3f viewvec = sphere_center - cam_location;
    vec2f phisical_screensize(2.0f, 2.0f);
    vec2u pixel_screensize(WINDOW_WIDTH, WINDOW_HEIGHT);
    float fov = PI/3;
    Camera camera(cam_location, viewvec, phisical_screensize, pixel_screensize, fov);
//----------------------------------------------------------------------------------
    
    camera.Render(screenBuffer, scene); //rendering

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glViewport(0, 0, WINDOW_WIDTH , WINDOW_HEIGHT);                                             GL_CHECK_ERRORS;
        glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
        
    	glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    return 0;
}