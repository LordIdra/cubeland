#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//'Raise' a generic error which outputs line, file, error and then pauses the 
//program until a key is pressed
#define raise_error(error)\
    std::cout << "=====[ERROR]=====" << "\n";\
    std::cout << "FILE: " << __FILE__ << "\n";\
    std::cout << "LINE: " << __LINE__ << "\n";\
    std::cout << error << "\n";\
    system("pause");

//Initialize GLFW. If initialization fails, raise an error
void InitializeGLFW()
{
    if (!glfwInit())
    {
        raise_error("failed to initialize glfw");
        glfwTerminate();
    }
}

//Automatically handles setting OpenGL version and profile
void SetVersion(unsigned int major, unsigned int minor, bool use_core_profile)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
    if (use_core_profile) glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

//Initialize GLAD. If initialization fails, raise an error
void InitializeGLAD()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        raise_error("failed to initialize glad");
    }

}