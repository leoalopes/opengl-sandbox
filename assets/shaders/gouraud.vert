#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 normalMatrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 lightPos;

uniform vec3 objectColor;
uniform vec3 lightColor;

out vec3 fragColor;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    vec3 fragPos = vec3(view * model * vec4(aPos, 1.0));

    vec3 normal = normalize(mat3(normalMatrix) * aNormal);
    vec3 lightPos = vec3(view * vec4(lightPos, 1.0));
    vec3 lightDirection = normalize(lightPos - fragPos);

    float ambientStrength = 0.1f;
    vec3 ambientLight = ambientStrength * lightColor;

    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 1.0;
    vec3 viewDirection = normalize(-fragPos);
    vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 4);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 color = (ambientLight + diffuse + specular) * objectColor;
    fragColor = color;
}
