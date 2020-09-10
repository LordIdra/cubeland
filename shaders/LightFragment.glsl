#version 330 core

out vec4 color;
in vec2 texture_coord;
uniform sampler2D shape_texture;

void main()
{
    //color = texture(shape_texture, texture_coord);
    color = texture(shape_texture, texture_coord);
}