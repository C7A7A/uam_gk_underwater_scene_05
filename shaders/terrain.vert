#version 400 core

vec3 getNormal();

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoords;

out DATA {
	vec3 position;
	vec3 fragPosition;
	vec3 surfaceNormal;
	//vec3 toLightVector;
	vec3 toCameraVector;
	vec2 texCoords;
} vs_out;

uniform mat4 projection;
uniform mat4 view = mat4(1.0);
uniform mat4 model = mat4(1.0);
//uniform vec3 lightPosition;

uniform sampler2D heightMapTexture;

uniform float AMPLITUDE;

// fog
uniform float density = 0.004;
uniform float gradient = 3.0;

out float visibility;


void main() {
	float height = texture(heightMapTexture, texCoords).r;
	
	height = height + pow(2, -height);
	height = height * AMPLITUDE;

	vec4 worldPosition = model * vec4(position.x, height, position.z, 1.0);

	gl_Position = projection * view * worldPosition;

	vs_out.position = vec3(position.x, height, position.z);
	vs_out.fragPosition = vec3(model * vec4(position, 1.0f));

	// Multiply it so the textures repeat, instead of stretching
	vs_out.texCoords = texCoords * 80;

	vs_out.surfaceNormal = getNormal();
	//vs_out.toLightVector = lightPosition - position;
	//vs_out.toLightVector = lightPosition - worldPosition.xyz;
	vs_out.toCameraVector = (inverse(view) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;

	// fog
	float distance = length(gl_Position.xyz);
	visibility = exp(-pow((distance*density), gradient));
	visibility = clamp(visibility,0.0,1.0);

}

vec3 getNormal() {
	const vec2 size = vec2(2.0, 0.0);
	const ivec3 offset = ivec3(-1, 0, 1);

	float s01 = textureOffset(heightMapTexture, texCoords, offset.xy).x;
	float s21 = textureOffset(heightMapTexture, texCoords, offset.zy).x;
    float s10 = textureOffset(heightMapTexture, texCoords, offset.yx).x;
    float s12 = textureOffset(heightMapTexture, texCoords, offset.yz).x;
	vec3 va = normalize(vec3(size.xy, s21 - s01));
	vec3 vb = normalize(vec3(size.yx, s12 - s10));

	return cross(va, vb);
}