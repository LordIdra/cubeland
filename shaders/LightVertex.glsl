#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 offset;
layout (location = 2) in vec2 texture_coordinates;

uniform mat4 camera_matrix;

out vec2 texture_coord;

void main()
{
    gl_Position = camera_matrix * vec4(position + offset, 1.0);
    texture_coord = texture_coordinates;
}