#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 cameraPos;

void main() {
    // ambient lighting
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;
    
    // diffuse lighting
    vec3 normalizedNormal = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - FragPos);
    vec3 diffuse = max(dot(normalizedNormal, lightDirection), 0.0f) * lightColor;

    // specular lighting
    float specularStrength = 0.5f;
    int shininess = 32;
    vec3 viewingDirection = normalize(cameraPos - FragPos);
    vec3 reflectionDirection = reflect(-lightDirection, normalizedNormal);
    float spec = pow(max(dot(viewingDirection, reflectionDirection), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0f);
}
