#version 330 core

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

struct Spotlight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

uniform Spotlight light;
uniform Material material;

void main() {
    // ambient lighting
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoord));
    
    // diffuse lighting
    vec3 normalizedNormal = normalize(Normal);
    vec3 lightDirection = normalize(light.position - FragPos);
    vec3 diffuse = light.diffuse * (max(dot(normalizedNormal, lightDirection), 0.0f) * vec3(texture(material.texture_diffuse1, TexCoord)));

    // specular lighting
    vec3 viewingDirection = normalize(-FragPos); // camera position in view space is the origin
    vec3 reflectionDirection = reflect(-lightDirection, normalizedNormal);
    float spec = pow(max(dot(viewingDirection, reflectionDirection), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.texture_specular1, TexCoord)));

    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance
                                               + light.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    // spotlight
    float theta = dot(lightDirection, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); 

    diffuse *= intensity;
    specular *= intensity;

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0f);
}
