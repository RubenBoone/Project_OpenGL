#version 330 core
#define MAX_LIGHTS 64

struct Light {
    vec3 position;
    vec4 color;

    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform sampler2D Text;
uniform float numOfLights;

uniform Light light[MAX_LIGHTS];

vec4 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{

    float ambientStrenght = 0.1;
    vec4 ambient = ambientStrenght * light.color;

    vec3 lightDir = normalize(light.position - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
        float diff = max(dot(normal, lightDir), 0.0);
    vec4 diffuse = diff * light.color;

    float specularStrenght = 0.5;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec4 specular = (specularStrenght * spec * light.color); 

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

    FragColor = result * texture(Text, TexCoord);
}