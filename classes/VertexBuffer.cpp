#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


struct VertexBuffer
{
    //We use normal arrays to store data since this is what OpenGL uses
    //So we need to store the size to ensure we don't accidentally access
    //memory that doesn't belong to us, should we want to do some sort of
    //more advanced function later on
    unsigned int ID = 0;
    unsigned int size = 0;

    //Generates and binds a new buffer, before loading provided data into it
    //We also store the size of the data. We cannot use sizeof(data) within the function
    //as that would simple return the size of a void pointer, not the array!
    VertexBuffer(unsigned int data_size, void* data, unsigned int operation)
    {
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ARRAY_BUFFER, ID);
        glBufferData(GL_ARRAY_BUFFER, data_size, data, operation);
        Unbind();

        size = data_size;
    }

    //Destructor removes the buffers
    //Should be used when we load a new scene, level etc
    ~VertexBuffer()
    {
        glDeleteBuffers(1, &ID);
    }

    //Comparable to macros. Inline because they are very small functions that could easily
    //be written out in full to the same effect
    inline void Bind() {glBindBuffer(GL_ARRAY_BUFFER, ID);}
    inline void Unbind() {glBindBuffer(GL_ARRAY_BUFFER, 0);}
};