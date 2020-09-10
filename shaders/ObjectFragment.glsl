#version 330 core

out vec4 color;

in vec2 texture_coord;
in vec3 fragment_position;
in vec3 normal;

uniform vec3 light_position;
uniform vec3 light_color;
uniform sampler2D shape_texture;

void main()
{
    vec3 ambient = vec3(0.05f, 0.05f, 0.05f);
    vec3 norm = normalize(normal);
    vec3 light_direction = normalize(light_position - fragment_position);

    float diff = max(dot(norm, light_direction), 0.0); //the lower the angle, the lower the light intensity
    vec3 diffuse = diff * light_color;

    vec3 light_intensity = (ambient + diffuse); //ambient + light intensity

    color = vec4(texture(shape_texture, texture_coord) * light_intensity, 1.0);
}