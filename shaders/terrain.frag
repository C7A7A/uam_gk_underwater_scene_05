#version 400 core

out vec4 fragColor;

struct Material
{
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};

struct Light
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    
    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in DATA {
	vec3 position;
	vec3 fragPosition;
	vec3 surfaceNormal;
	//vec3 toLightVector;
	vec3 toCameraVector;
	vec2 texCoords;
} fs_in;

uniform Light light;
uniform Material material;

uniform sampler2D tex;
uniform sampler2D heightMapTexture;
//uniform vec3 lightColor;
//uniform float shineDamper = 1;
//uniform float reflectivity = 0.0;

// fog
in float visibility;

void main() {
	// normalize into vector space
	vec3 normal = fs_in.surfaceNormal;
	
	// float r = normal.x;
	// float g = normal.y;
	// float b = normal.z;
	// float a = 1; 

	// Light calculations
	//vec3 unitNormal = vec3(r, g, b);
	// vec3 unitLightVector = normalize(fs_in.toLightVector);

	//float nDotl = dot(unitNormal, unitLightVector);
	//float brightness = max(nDotl, 0.2);
	//vec3 diffuse = brightness * lightColor;
	
	// Ambient
    vec3 ambient = light.ambient * texture(material.diffuse, fs_in.texCoords).rgb;
	
	// Diffuse
	vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fs_in.fragPosition);
    float diff = max(dot(norm, lightDir), 0.0) * 2.0f;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, fs_in.texCoords).rgb;

	// Spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;

	// Attenuation
    float distance = length(light.position - fs_in.fragPosition);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    diffuse *= attenuation;

	vec3 result = ambient + diffuse;

	// vec3 unitVectorToCamera = normalize(fs_in.toCameraVector);
	// vec3 lightDirection = -unitLightVector;
	// vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);

	// float specularFactor = dot(reflectedLightDirection, unitVectorToCamera);
	// specularFactor = max(specularFactor, 0.0);
	// float dampedFactor = pow(specularFactor, shineDamper);
	// vec3 finalSpecular = dampedFactor * reflectivity * lightColor;

	//fragColor = vec4(brightness) * vec4(lightColor, 1.0) * texture(tex, fs_in.texCoords);
	
	fragColor = vec4(result, 1.0);
	
	fragColor = mix(vec4(0.02, 0.05, 0.28, 1.0), fragColor, visibility); // mg³a

	//fragColor = vec4(r, g, b, a);
}
