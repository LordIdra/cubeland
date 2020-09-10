#include <iostream>
#include <fstream>
#include <sstream>

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

struct Shader
{
    unsigned int ID = 0;

    //Initializes the program by loading vertex and fragment shaders, compiling them
    //and loading them into a program
    Shader(std::string vertex_path, std::string fragment_path)
    {
        //Open shader files
        std::ifstream vertex_source_file(vertex_path);
        std::ifstream fragment_source_file(fragment_path);

        //Read files into stringstreams
        std::stringstream vertex_source_buffer;
        std::stringstream fragment_source_buffer;

        vertex_source_buffer << vertex_source_file.rdbuf();
        fragment_source_buffer << fragment_source_file.rdbuf();

        //Close files
        vertex_source_file.close();
        fragment_source_file.close();

        //Read stringstreams into strings
        const std::string vertex_source_string = (vertex_source_buffer.str());
        const std::string fragment_source_string = (fragment_source_buffer.str());

        //Convert strings to char arrays
        const char *vertex_source = vertex_source_string.c_str();
        const char *fragment_source = fragment_source_string.c_str();

        //Create shader objects
        unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

        //Initialize shader objects
        glShaderSource(vertex_shader, 1, &vertex_source, NULL);
        glShaderSource(fragment_shader, 1, &fragment_source, NULL);

        //Variables to check compilation status
        int  shader_compile_success;
        char shader_compile_log[512];
        
        //Compile vertex shader
        glCompileShader(vertex_shader);
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &shader_compile_success);
        if (!shader_compile_success)
        {
            glGetShaderInfoLog(vertex_shader, 512, NULL, shader_compile_log);
            raise_error("VERTEX SHADER\n" << shader_compile_log);
        }

        //Compile fragment shader
        glCompileShader(fragment_shader);
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &shader_compile_success);
        if (!shader_compile_success)
        {
            glGetShaderInfoLog(fragment_shader, 512, NULL, shader_compile_log);
            raise_error("FRAGMENT SHADER\n" << shader_compile_log);
        }

        //Variables to check program initialization status
        int  shader_init_success;
        char shader_init_log[512];

        //Create and initialize shader program
        unsigned int program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);

        glGetProgramiv(program, GL_LINK_STATUS, &shader_init_success);
        if(!shader_init_success) {
            glGetProgramInfoLog(program, 512, NULL, shader_init_log);
            raise_error("PROGRAM INIT\n" << shader_init_log);
        }

        //Delete shader objects, since they're now attached to the program
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        //Return our final shader
        ID = program;
    }

    //Inline because it's comparable to a macro and easily substituted
    inline void Use()
    {
        glUseProgram(ID);
    }

    //Finds and returns the location of a uniform
    unsigned int UniformLocation(const char* name)
    {
        return glGetUniformLocation(ID, name);
    }
};