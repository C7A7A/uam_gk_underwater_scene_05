#version 400 core

uniform vec3 objectColor;
uniform vec3 lightDir;

in vec3 interpNormal;

// mg³a
in float visibility;

void main()
{
	vec3 normal = normalize(interpNormal);
	float diffuse = max(dot(normal, -lightDir), 0.0);
	gl_FragColor = vec4(objectColor * diffuse, 1.0);
	gl_FragColor = mix(vec4(0.03, 0.07, 0.38, 1.0), gl_FragColor, visibility); // mg³a
}
