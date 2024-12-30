#version 330 core

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
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

    float enabled;
};

struct PointLight {
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
in vec2 TexCoord;

out vec4 FragColor;

uniform Material material;
uniform DirectionalLight directionalLight;
uniform SpotLight spotLight;
#define NR_POINT_LIGHTS 1
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewingDirection);
vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewingDirection);
vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 viewingDirection);

void main() {
    vec4 texColor = texture(material.texture_diffuse1, TexCoord);
    if (texColor.a < 0.1f) {
        discard;
    }

    vec3 normalizedNormal = normalize(Normal);
    vec3 viewingDirection = normalize(-FragPos); // camera position in view space is the origin

    vec3 result = calculateDirectionalLight(directionalLight, normalizedNormal, viewingDirection);

    for (int i = 0; i < NR_POINT_LIGHTS; ++i) {
        result += calculatePointLight(pointLights[i], normalizedNormal, viewingDirection);
    }

    result += calculateSpotLight(spotLight, normalizedNormal, viewingDirection);

    FragColor = vec4(result, 1.0f);
}

vec3 calculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewingDirection) {
    // ambient lighting
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoord));
    
    // diffuse lighting
    vec3 lightDirection = normalize(-light.direction);
    vec3 diffuse = light.diffuse * (max(dot(normal, lightDirection), 0.0f) * vec3(texture(material.texture_diffuse1, TexCoord)));

    // specular lighting
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewingDirection, reflectionDirection), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.texture_specular1, TexCoord)));

    return ambient + diffuse + specular;
}

vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 viewingDirection) {
    // ambient lighting
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoord));
    
    // diffuse lighting
    vec3 lightDirection = normalize(light.position - FragPos);
    vec3 diffuse = light.diffuse * (max(dot(normal, lightDirection), 0.0f) * vec3(texture(material.texture_diffuse1, TexCoord)));

    // specular lighting
    vec3 reflectionDirection = reflect(-lightDirection, normal);
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

    diffuse *= intensity * light.enabled;
    specular *= intensity * light.enabled;

    return ambient + diffuse + specular;
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 viewingDirection) {
    // ambient lighting
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoord));
    
    // diffuse lighting
    vec3 lightDirection = normalize(light.position - FragPos);
    vec3 diffuse = light.diffuse * (max(dot(normal, lightDirection), 0.0f) * vec3(texture(material.texture_diffuse1, TexCoord)));

    // specular lighting
    vec3 reflectionDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewingDirection, reflectionDirection), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * vec3(texture(material.texture_specular1, TexCoord)));

    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance
                                               + light.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    return ambient + diffuse + specular;
}
