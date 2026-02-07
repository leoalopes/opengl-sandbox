#version 330 core

struct Material {
    sampler2D baseColor;
    sampler2D metallicRoughness;

    float shininess;

    bool hasEmissiveColor;
    sampler2D emissiveColor;

    bool hasNormalTexture;
    sampler2D normalTexture;
};

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;

    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

in GeometryData {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
} geometryInput;

uniform vec3 cameraPosition;

uniform DirectionalLight directionalLight;

uniform int nPointLights;
uniform PointLight pointLights[4];

uniform int nSpotLights;
uniform SpotLight spotLights[4];

uniform Material material;

uniform samplerCube environment;

out vec4 FragColor;

vec3 calculateDirectionalLight(
    vec3 toTheCamera,
    vec3 normal,
    vec3 diffuseColor,
    vec3 specularColor,
    float shininess,
    DirectionalLight light
);

vec3 calculatePointLight(
    vec3 toTheCamera,
    vec3 position,
    vec3 normal,
    vec3 diffuseColor,
    vec3 specularColor,
    float shininess,
    PointLight light
);

vec3 calculateSpotLight(
    vec3 toTheCamera,
    vec3 position,
    vec3 normal,
    vec3 diffuseColor,
    vec3 specularColor,
    float shininess,
    SpotLight light
);

void main() {
    vec3 normal = normalize(geometryInput.Normal);
    vec3 toTheCamera = normalize(cameraPosition - geometryInput.Position);

    vec3 metallicRoughness = vec3(texture(material.metallicRoughness, geometryInput.TexCoords));
    float metalness = metallicRoughness.b;
    float roughness = metallicRoughness.g;
    vec3 specularColor = vec3(1.0) * (1 - roughness);
    vec4 textureColor = texture(material.baseColor, geometryInput.TexCoords);

    vec3 refractionDirection = refract(-toTheCamera, normal, 1 - roughness);
    vec3 reflectionDirection = reflect(refractionDirection, normal);
    vec4 environmentReflection = texture(environment, reflectionDirection);

    vec3 diffuseColor = vec3(textureColor) * (1 - metalness) + metalness * vec3(environmentReflection);
    vec3 radiance = vec3(0.0);
    radiance += calculateDirectionalLight(
        toTheCamera,
        normal,
        diffuseColor,
        specularColor,
        material.shininess,
        directionalLight
    );

    for (int i = 0; i < nPointLights; i++) {
        radiance += calculatePointLight(
            toTheCamera,
            geometryInput.Position,
            normal,
            diffuseColor,
            specularColor,
            material.shininess,
            pointLights[i]
        );
    }

    for (int i = 0; i < nSpotLights; i++) {
        radiance += calculateSpotLight(
            toTheCamera,
            geometryInput.Position,
            normal,
            diffuseColor,
            specularColor,
            material.shininess,
            spotLights[i]
        );
    }

    if (material.hasEmissiveColor) {
        vec3 emissiveColor = vec3(texture(material.emissiveColor, geometryInput.TexCoords));
        radiance += emissiveColor;
    }

    FragColor = vec4(radiance, textureColor.a);
}

vec3 calculateDirectionalLight(
    vec3 toTheCamera,
    vec3 normal,
    vec3 diffuseColor,
    vec3 specularColor,
    float shininess,
    DirectionalLight light
) {
    vec3 fromTheLight = normalize(light.direction);

    vec3 ambient = diffuseColor * light.ambient;
    vec3 diffuse =  diffuseColor * light.diffuse;

    vec3 halfwayVector = normalize(-fromTheLight + toTheCamera);
    float specStrength = pow(max(dot(halfwayVector, normal), 0.0), shininess);
    vec3 specular = specStrength * specularColor * light.specular;

    float geometryTerm = max(dot(normal, -fromTheLight), 0.0);
    return ambient + (diffuse + specular) * geometryTerm;
}

vec3 calculatePointLight(
    vec3 toTheCamera,
    vec3 position,
    vec3 normal,
    vec3 diffuseColor,
    vec3 specularColor,
    float shininess,
    PointLight light
) {
    float distance = length(position - light.position);
    float attenuation = 1.0 / (
        light.constant + light.linear * distance + light.quadratic * (distance * distance)
    ); 

    vec3 fromTheLight = normalize(position - light.position);

    vec3 diffuse = diffuseColor * light.diffuse;

    vec3 halfwayVector = normalize(-fromTheLight + toTheCamera);
    float specStrength = pow(max(dot(halfwayVector, normal), 0.0), shininess);
    vec3 specular = specStrength * specularColor * light.specular;

    diffuse *= attenuation;
    specular *= attenuation;

    float geometryTerm = max(dot(normal, -fromTheLight), 0.0);
    return (diffuse + specular) * geometryTerm;
}

vec3 calculateSpotLight(
    vec3 toTheCamera,
    vec3 position,
    vec3 normal,
    vec3 diffuseColor,
    vec3 specularColor,
    float shininess,
    SpotLight light
) {
    float distance = length(position - light.position);
    float attenuation = 1.0 / (
        light.constant + light.linear * distance + light.quadratic * (distance * distance)
    ); 

    vec3 fromTheLight = normalize(position - light.position);
    vec3 direction = normalize(light.direction);

    vec3 diffuse = diffuseColor * light.diffuse;

    vec3 halfwayVector = normalize(-fromTheLight + toTheCamera);
    float specStrength = pow(max(dot(halfwayVector, normal), 0.0), shininess);
    vec3 specular = specStrength * specularColor * light.specular;

    float lightAngle = dot(fromTheLight, direction);
    float intensity = clamp((lightAngle - light.outerCutOff) / (light.cutOff - light.outerCutOff), 0.0, 1.0);

    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    float geometryTerm = max(dot(normal, -fromTheLight), 0.0);
    return (diffuse + specular) * geometryTerm;
}
