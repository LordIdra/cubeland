#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//'Raise' a generic error which outputs line, file, error and then pauses the 
//program until a key is pressed
#define raise_gl_error(error, file, line)\
    std::cout << "=====[ERROR]=====" << "\n";\
    std::cout << "FILE: " << file << "\n";\
    std::cout << "LINE: " << line << "\n";\
    std::cout << error << "\n";\
    system("pause");

//Inputs LINE and FILE - macros are essentially substituted, so the __LINE__ and
//__FILE__ parameters end up being inputted from the line/file the macro is used
#define get_errors()\
    get_gl_errors(__FILE__, __LINE__);

//Print all the OpenGL errors, clearing the error buffer as we go
void get_gl_errors(std::string file, unsigned int line)
{
    unsigned int errorCode;
    
    //Loop through all errors until an error code is returned
    while ((errorCode = glGetError()) != 0)
    {
        std::string error;
        switch (errorCode)
        {
            //Converts error codes into readable human format
            case 1280:      error = "invalid enum"; break;
            case 1281:      error = "invalid value"; break;
            case 1282:      error = "invalid operation"; break;
            case 1283:      error = "stack overflow"; break;
            case 1284:      error = "stack underflow"; break;
            case 1285:      error = "out of memory"; break;
            case 1286:      error = "invalid framebuffer operation"; break;
        }
        raise_gl_error(error, file, line);
    }
}