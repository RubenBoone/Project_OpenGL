#version 330 core

struct Light {	
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
uniform Light light;

void main()
{
    float ambientStrenght = 0.1;
    vec4 ambient = ambientStrenght * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec4 diffuse = diff * lightColor;

    float specularStrenght = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec4 specular = (specularStrenght * spec * lightColor); 

    float distance    = length(lightPos - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    ambient  *= attenuation;  
    diffuse  *= attenuation;
    specular *= attenuation; 

    vec4 result = (ambient + diffuse + specular) * texture(Text, TexCoord);
    FragColor = result;
}