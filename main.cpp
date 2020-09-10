#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <functions/Initialize.cpp>
#include <functions/Debug.cpp>

#include <classes/WorldObject.cpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define raise_error(error)\
    std::cout << "=====[ERROR]=====" << "\n";\
    std::cout << "FILE: " << __FILE__ << "\n";\
    std::cout << "LINE: " << __LINE__ << "\n";\
    std::cout << error << "\n";\
    system("pause");

/*
 * AUXILIARY FUNCTIONS
 */

//Resizes window and scales rendering operations accordingly
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

//Camera variables
float delta_time = 0.0f;
float last_frame = 0.0f;
bool first_mouse = true;

Camera camera;

//Process user input (ie; keyboard/mouse input)
void processInput(GLFWwindow *window)
{
    float current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;  
    float camera_speed = 2.5f * delta_time;
    float sensitivity = 0.1f;

    glm::vec3 translate_vector = glm::vec3(0.0f, 0.0f, 0.0f);
    
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        translate_vector.x = camera_speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        translate_vector.x = -camera_speed;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        translate_vector.z = -camera_speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        translate_vector.z = camera_speed;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        translate_vector.y = camera_speed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        translate_vector.y = -camera_speed;

    camera.Translate(translate_vector);
}

void processMouse(GLFWwindow* window, double x_pos, double y_pos)
{
    float sensitivity = 0.1f;
    int width = 0;
    int height = 0;

    glfwGetWindowSize(window, &width, &height);

    float centre_x = width/2;
    float centre_y = height/2;

    glfwSetCursorPos(window, centre_x, centre_y);

    if (first_mouse) // initially set to true
    {
        centre_x = x_pos;
        centre_y = y_pos;
        first_mouse = false;
    }

    float x_offset = x_pos - centre_x;
    float y_offset = centre_y - y_pos;

    x_offset *= sensitivity;
    y_offset *= sensitivity;

    float yaw = x_offset;
    float pitch = y_offset;

    std::cout << yaw << "\n";
    std::cout << pitch << "\n\n";

    camera.Rotate(glm::vec3(yaw, pitch, 0.0f));
}


/*
 * MAIN FUNCTION
 */
int main()
{

    /* INITIALIZE GLFW/GLAD/SHADERS AND CREATE WINDOW */
    InitializeGLFW();
    SetVersion(3, 3, true);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
    if (window == NULL)
    {
        raise_error("failed to create window");
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
    glfwSetCursorPosCallback(window, processMouse);  

    InitializeGLAD();

    Shader object_program("shaders/ObjectVertex.glsl", "shaders/ObjectFragment.glsl");
    Shader light_program("shaders/LightVertex.glsl", "shaders/LightFragment.glsl");


    /* INITIALIZE BUFFERS */
    float cube_vertices[] = {
        -0.5f, -0.5f, -0.5f,        0.0f, 0.0f,         0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,         1.0f, 0.0f,         0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,         1.0f, 1.0f,         0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,         1.0f, 1.0f,         0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,        0.0f, 1.0f,         0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,        0.0f, 0.0f,         0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,        0.0f, 0.0f,         0.0f,  0.0f, 1.0f,    
        0.5f, -0.5f,  0.5f,         1.0f, 0.0f,         0.0f,  0.0f, 1.0f,        
        0.5f,  0.5f,  0.5f,         1.0f, 1.0f,         0.0f,  0.0f, 1.0f,  
        0.5f,  0.5f,  0.5f,         1.0f, 1.0f,         0.0f,  0.0f, 1.0f,  
        -0.5f,  0.5f,  0.5f,        0.0f, 1.0f,         0.0f,  0.0f, 1.0f,  
        -0.5f, -0.5f,  0.5f,        0.0f, 0.0f,         0.0f,  0.0f, 1.0f,  

        -0.5f,  0.5f,  0.5f,        1.0f, 0.0f,         -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,        1.0f, 1.0f,         -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,        0.0f, 1.0f,         -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,        0.0f, 1.0f,         -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,        0.0f, 0.0f,         -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,        1.0f, 0.0f,         -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,         1.0f, 0.0f,         1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,         1.0f, 1.0f,         1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,         0.0f, 1.0f,         1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,         0.0f, 1.0f,         1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,         0.0f, 0.0f,         1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,         1.0f, 0.0f,         1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,        0.0f, 1.0f,         0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,         1.0f, 1.0f,         0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,         1.0f, 0.0f,         0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,         1.0f, 0.0f,         0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,        0.0f, 0.0f,         0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,        0.0f, 1.0f,         0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,        0.0f, 1.0f,         0.0f, 1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,         1.0f, 1.0f,         0.0f, 1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,         1.0f, 0.0f,         0.0f, 1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,         1.0f, 0.0f,         0.0f, 1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,        0.0f, 0.0f,         0.0f, 1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,        0.0f, 1.0f,         0.0f, 1.0f,  0.0f
    };

    //0 = position, 1 = color
    WorldObject cube(object_program, cube_vertices, sizeof(cube_vertices), 36);
    cube.vertex_buffer.Bind();
    cube.AddVertexAttribute(0, 3, GL_FLOAT, false, 8 * sizeof(float), 0);
    cube.AddVertexAttribute(2, 2, GL_FLOAT, false, 8 * sizeof(float), 3 * sizeof(float));
    cube.AddVertexAttribute(3, 3, GL_FLOAT, false, 8 * sizeof(float), 5 * sizeof(float));
    cube.vertex_buffer.Unbind();
    cube.SetInstanceCount(10000);

    WorldObject light(light_program, cube_vertices, sizeof(cube_vertices), 36);
    light.vertex_buffer.Bind();
    light.AddVertexAttribute(0, 3, GL_FLOAT, false, 8 * sizeof(float), 0);
    light.AddVertexAttribute(2, 2, GL_FLOAT, false, 8 * sizeof(float), 3 * sizeof(float));
    cube.AddVertexAttribute(3, 3, GL_FLOAT, false, 8 * sizeof(float), 5 * sizeof(float));
    light.vertex_buffer.Unbind();
    light.SetInstanceCount(1);

    //Now we need to load some textures and use our new function to create an array
    //of textures to use for each triangle
    Texture2D grass_texture("textures/grass.JPG", GL_RGB);
    Texture2D wood_texture("textures/wood.JPG", GL_RGB);

    //Generate cube positions
    std::vector<glm::vec3> cube_positions;
    for (float x = 0; x < 100; x++)
    {
        for (float z = 0; z < 100; z++)
        {
            cube_positions.push_back(glm::vec3(x, (float)(rand() % 5)/20.0f, z));
        }
    }

    //Light positions
    std::vector<glm::vec3> light_positions {
        glm::vec3(5.0f, 2.0f, 5.0f)
    };


    /* MAIN LOOP */
    while(!glfwWindowShouldClose(window))
    {
        /* PROCESS INPUT */
        processInput(window);


        /* RENDERING */
        //Clear screen
        glClearColor(0.05f, 0.05f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Projection matrices/settings
        glEnable(GL_DEPTH_TEST);  

        //Create camera matrix
        glm::mat4 projection_matrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 1000.0f);;
        glm::mat4 view_matrix = camera.GetViewMatrix();
        glm::mat4 camera_matrix = projection_matrix * view_matrix;

        //Draw objects
        cube.program.Use();
        cube.vertex_array.Bind();
        cube.AddInstanceAttribute(1, 3, cube_positions.size() * sizeof(glm::vec3), &cube_positions[0]);
        grass_texture.Bind();
        glUniformMatrix4fv(cube.program.UniformLocation("camera_matrix"), 1, GL_FALSE, glm::value_ptr(camera_matrix));
        glUniform3f(cube.program.UniformLocation("light_position"), 5.0f, 2.0f, 5.0f);
        glUniform3f(cube.program.UniformLocation("light_color"), 1.0f, 1.0f, 1.0f);
        cube.Draw();

        //Draw lights
        light.program.Use();
        light.vertex_array.Bind();
        light.AddInstanceAttribute(1, 3, light_positions.size() * sizeof(glm::vec3), &light_positions[0]);
        wood_texture.Bind();
        glUniformMatrix4fv(light.program.UniformLocation("camera_matrix"), 1, GL_FALSE, glm::value_ptr(camera_matrix));
        light.Draw();


        /* FINAL OPERATIONS */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    /* WHEN WINDOW TERMINATED */
    glfwTerminate();
    return 0;
}