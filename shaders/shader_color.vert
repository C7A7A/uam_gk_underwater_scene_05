#version 400 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexTexCoord;
layout(location = 2) in vec3 vertexNormal;

uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;

out vec3 interpNormal;

// fog
uniform float density = 0.004;
uniform float gradient = 3.0;

out float visibility;

void main()
{
	gl_Position = modelViewProjectionMatrix * vec4(vertexPosition, 1.0);
	interpNormal = (modelMatrix * vec4(vertexNormal, 0.0)).xyz;

	// fog
	float distance = length(gl_Position.xyz);
	visibility = exp(-pow((distance*density), gradient));
	visibility = clamp(visibility,0.0,1.0);
}
