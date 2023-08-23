#version 400 core
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

in vec3 FragPos;
in vec3 Normal;


uniform vec3 viewPos;
uniform vec3 objectColor;
uniform Material material;
uniform Light light;

// fog
in float visibility;

void main()
{

    // Ambient
    vec3 ambient = light.ambient;
    
    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0) * 2.0f;
    vec3 diffuse = light.diffuse * diff;

    // Spotlight (soft edges)
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse  *= intensity;
    
    // Attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    diffuse  *= attenuation;

    vec3 result = objectColor * (ambient + diffuse);    
    gl_FragColor = vec4(result, 1.0);
    gl_FragColor = mix(vec4(0.02, 0.05, 0.28, 1.0), gl_FragColor, visibility); // fog
}
