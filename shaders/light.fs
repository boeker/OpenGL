#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform Light light;
uniform Material material;

void main() {
    // ambient lighting
    vec3 ambient = light.ambient * material.ambient;
    
    // diffuse lighting
    vec3 normalizedNormal = normalize(Normal);
    vec3 lightDirection = normalize(light.position - FragPos);
    vec3 diffuse = light.diffuse * (max(dot(normalizedNormal, lightDirection), 0.0f) * material.diffuse);

    // specular lighting
    vec3 viewingDirection = normalize(-FragPos); // camera position in view space is the origin
    vec3 reflectionDirection = reflect(-lightDirection, normalizedNormal);
    float spec = pow(max(dot(viewingDirection, reflectionDirection), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance
                                               + light.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0f);
}
