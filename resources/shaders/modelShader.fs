#version 330 core
#define MAX_LIGHTS 64

struct Light {
    vec3 position;

    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D Text;
uniform float numOfLights;

uniform Light light[MAX_LIGHTS];

out vec4 color;

uniform sampler2D texture_diffuse;

vec4 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{

    float ambientStrenght = 1.0;
    vec4 ambient = ambientStrenght * lightColor;

    vec3 lightDir = normalize(light.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
        float diff = max(dot(normal, lightDir), 0.0);
    vec4 diffuse = diff * lightColor;

    float specularStrenght = 0.5;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec4 specular = (specularStrenght * spec * lightColor); 

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient  *= attenuation;  
    diffuse  *= attenuation;
    specular *= attenuation; 

    return (ambient + diffuse + specular);
}

void main()
{
    vec3 norm = normalize(Normal);

    vec3 viewDir = normalize(viewPos - FragPos);

    vec4 result;

    for (int i = 0; i < numOfLights; i++)
        result += CalcPointLight(light[i], norm, FragPos, viewDir);

    color = result * texture(texture_diffuse, TexCoords);
}