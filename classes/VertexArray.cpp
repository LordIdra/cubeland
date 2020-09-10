#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


struct VertexArray
{
    //It's important we have vertex_buffer already declared
    //If we declare it within a function, it will be deleted at the end of the function call!!!
    unsigned int VAO_ID = 0;

    //Generates one vertex array and binds it under the assumption the following operations will
    //be on this vertex array
    VertexArray()
    {
        glGenVertexArrays(1, &VAO_ID);
        glBindVertexArray(VAO_ID);
    }

    //Destructor removes the vertex array and all associated buffers
    //Should be used when we load a new scene, level etc
    //Note that deleting the VAO does NOT delete the VBO as well
    ~VertexArray()
    {
        glDeleteVertexArrays(1, &VAO_ID);
    }

    //Comparable to macros. Inline following the logic that they are very 
    //small functions that could easily be written out in full to the same effect
    inline void Bind() {glBindVertexArray(VAO_ID);}
    inline void Unbind() {glBindVertexArray(0);}

    void AddVertexAttribute(unsigned int layout_position, unsigned int byte_size, unsigned int type,
        bool normalized, unsigned int byte_stride, unsigned int byte_offset)
    {
        //Binds the vertex array to be sure that we're actually making attribute pointers
        //for the correct VBO
        glBindVertexArray(VAO_ID);
        glVertexAttribPointer(
            layout_position, 
            byte_size, 
            type, 
            normalized, 
            byte_stride, 
            (void*)byte_offset);
        
        glEnableVertexAttribArray(layout_position);
        glBindVertexArray(0);
    }
};