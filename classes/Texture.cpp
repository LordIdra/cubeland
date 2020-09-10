#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


unsigned int GetDimensionParameter(unsigned int dimensions)
{
    switch (dimensions) 
    {
    case 1: return GL_TEXTURE_1D;
    case 2: return GL_TEXTURE_2D;
    case 3: return GL_TEXTURE_3D;
    }

    return 0;
}


struct Texture2D
{
    unsigned int texture_ID;

    //Loads a texture file and creates a texture off that
    //The ID of the texture is stored for future use
    Texture2D(const char *file_path, unsigned int input_format)
    {
        //Image settings
        stbi_set_flip_vertically_on_load(true); 

        //Load texture file
        int width, height, nrChannels;
        unsigned char *data = stbi_load(file_path, &width, &height, &nrChannels, 0);

        //Generate texture objects
        glGenTextures(1, &texture_ID);
        glBindTexture(GL_TEXTURE_2D, texture_ID);

        //Generate textures + mipmaps
        glTexImage2D(
            GL_TEXTURE_2D,      //Bound texture to target
            0,                  //Mipmap level
            GL_RGB,             //Output format
            width,              //Width
            height,             //Height
            0,                  //Should always be 0 (deprecated)
            input_format,       //Input format
            GL_UNSIGNED_BYTE,   //Storage datatype
            data);              //Image itself

        //Generates all minimaps needed for currently bound texture
        glGenerateMipmap(GL_TEXTURE_2D);

        //Free memory which stored the image
        stbi_image_free(data);
    }

    //Deletes the texture from memory
    //Not as important as deleting VBOs/EBOs/VAOs since textures are more universal
    ~Texture2D()
    {
        glDeleteTextures(1, &texture_ID);
    }

    //Comparable to macros, so inline
    inline void Bind() {glBindTexture(GL_TEXTURE_2D, texture_ID);}
    inline void Unbind() {glBindTexture(GL_TEXTURE_2D, 0);}

    //Sets how the texture is repeated across a triangle
    void SetWrapMode(unsigned int dimensions, unsigned int s_mode, unsigned int t_mode)
    {
        Bind();
        unsigned int dimension_parameter = GetDimensionParameter(dimensions);
        glTexParameteri(dimension_parameter, GL_TEXTURE_WRAP_S, s_mode);
        glTexParameteri(dimension_parameter, GL_TEXTURE_WRAP_T, t_mode);
        Unbind();
    }

    //Sets how the texture pixels blend together
    void SetFilterMode(unsigned int dimensions, unsigned int mimify_mode, unsigned int magnify_mode)
    {
        Bind();
        unsigned int dimension_parameter = GetDimensionParameter(dimensions);
        glTexParameteri(dimension_parameter, GL_TEXTURE_MIN_FILTER, mimify_mode);
        glTexParameteri(dimension_parameter,  GL_TEXTURE_MAG_FILTER, magnify_mode);
        Unbind();
    }

    //Sets how different sizes of textures are calculated
    void SetMipmapMode(unsigned int dimensions, unsigned int mimify_mode, unsigned int magnify_mode)
    {
        Bind();
        unsigned int dimension_parameter = GetDimensionParameter(dimensions);
        glTexParameteri(dimension_parameter, GL_TEXTURE_MIN_FILTER, mimify_mode);
        glTexParameteri(dimension_parameter, GL_TEXTURE_MAG_FILTER, magnify_mode);
        Unbind();
    }
};