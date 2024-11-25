#version 330 core

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

struct Light {
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

uniform Light light;
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

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0f);
}
