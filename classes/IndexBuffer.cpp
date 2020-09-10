#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


struct IndexBuffer
{
    unsigned int ID = 0;
    unsigned int size = 0;
    
    //Generates and binds a new buffer, before loading provided data into it
    //We also store the size of the data. We cannot use sizeof(data) within the function
    //as that would simple return the size of a void pointer, not the array!
    IndexBuffer(unsigned int data_size, void* data, unsigned int operation)
    {
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_size, data, operation);

        size = sizeof(data);
    }

    //Destructor removes the index array and all associated buffers
    //Should be used when we load a new scene, level etc
    ~IndexBuffer()
    {
        glDeleteBuffers(1, &ID);
    }

    //Comparable to macros. Inline following the logic that they are very 
    //small functions that could easily be written out in full to the same effect
    inline void Bind() {glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);}
    inline void Unbind() {glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);}
};