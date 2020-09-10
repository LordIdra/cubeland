#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 offset;
layout (location = 2) in vec2 texture_coordinates;
layout(location = 3) in vec3 normal_vector;

uniform mat4 camera_matrix;

out vec2 texture_coord;
out vec3 normal;
out vec3 fragment_position;

void main()
{
    gl_Position = camera_matrix * vec4(position + offset, 1.0);
    texture_coord = texture_coordinates;
    normal = normal_vector;
    fragment_position = position + offset;
}