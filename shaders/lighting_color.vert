#version 400 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;
layout (location = 2) in vec3 normal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// fog
uniform float density = 0.004;
uniform float gradient = 3.0;

out float visibility;

void main()
{
    FragPos = vec3(model * vec4(position, 1.0f));
    Normal = mat3(transpose(inverse(model))) * normal;

    gl_Position = projection * view *  model * vec4(position, 1.0f);

    // fog
	float distance = length(gl_Position.xyz);
	visibility = exp(-pow((distance*density), gradient));
	visibility = clamp(visibility,0.0,1.0);
}