#version 400 core
layout (location = 0) in vec3 vertexPosition;

out vec3 texCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;

void main()
{
    vec4 pos = projectionMatrix * viewMatrix * vec4(vertexPosition, 1.0f);
    gl_Position = vec4(pos.x, pos.y, pos.w, pos.w);
    texCoords = vec3(vertexPosition.x, vertexPosition.y, -vertexPosition.z);

}    