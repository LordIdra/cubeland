#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <classes/Shader.cpp>
#include <classes/VertexBuffer.cpp>
#include <classes/IndexBuffer.cpp>
#include <classes/VertexArray.cpp>
#include <classes/Texture.cpp>
#include <classes/Camera.cpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//I'm not going to lie, this is some of the most horrible code I have ever seen
//but I couldn't think of another way to handle uniforms

struct WorldObject
{
    //Generic variables describing the object
    VertexArray vertex_array;
    VertexBuffer vertex_buffer;
    Shader program;

    //How many instances we want to create
    unsigned int instance_count = 0;

    //How many vertices we're using
    unsigned int vertex_count = 0;

    //Vector to store locations of instance attributes
    std::vector<unsigned int> instance_locations;

    //Creator VAO, VBO, EBO and shaders using provided information
    WorldObject(Shader shader, void* vertices, unsigned int vertices_size, unsigned int vertices_count)
        : vertex_array(VertexArray()),
        vertex_buffer(VertexBuffer(vertices_size, vertices, GL_STATIC_DRAW)),
        program(shader),
        vertex_count(vertices_count) {}

    //Lets us assign a vertex attribute, which is stored in our VAO
    void AddVertexAttribute(unsigned int layout_position, unsigned int byte_size, unsigned int type, 
        bool normalized, unsigned int byte_stride, unsigned int byte_offset)
    {
        vertex_array.AddVertexAttribute(layout_position, byte_size, type, normalized, byte_stride, byte_offset);
    }

    //Lets us assign an instance attribute
    void AddInstanceAttribute(unsigned int layout_position, unsigned int float_count, unsigned int data_size, void* data)
    {
        unsigned int instance_VBO;
        glGenBuffers(1, &instance_VBO);
        glBindBuffer(GL_ARRAY_BUFFER, instance_VBO);
        glBufferData(GL_ARRAY_BUFFER, data_size, data, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glEnableVertexAttribArray(layout_position);
        glBindBuffer(GL_ARRAY_BUFFER, instance_VBO);
        glVertexAttribPointer(layout_position, float_count, GL_FLOAT, GL_FALSE, float_count * sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glVertexAttribDivisor(layout_position, 1);

        instance_locations.push_back(instance_VBO);
    }
    
    //Set how many instances we want to draw
    void SetInstanceCount(unsigned int count) {instance_count = count;}

    //Bind all buffers and draw the objects
    void Draw()
    {
        vertex_array.Bind();
        vertex_buffer.Bind();

        glDrawArraysInstanced(GL_TRIANGLES, 0, vertex_count, instance_count);

        for (int i = 0; i < instance_locations.size(); i++)
        {
            glDeleteBuffers(1, &instance_locations[i]);
        }
        instance_locations.clear();
    }
};