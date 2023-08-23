#version 400 core

uniform vec3 objectColor;
uniform vec3 lightDir;
uniform sampler2D sampler;

in vec3 interpNormal;
in vec2 vertexUV;

void main()
{
	vec3 normal = normalize(interpNormal);
	float diffuse = max(dot(normal, -lightDir), 0.0);
	
	vec4 textureColor = texture2D(sampler, vertexUV);
	
	gl_FragColor = vec4(textureColor * diffuse);
}
