#version 330 core

struct Material {
    sampler2D baseColor;
};

in GeometryData {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
} geometryInput;

uniform Material material;

out vec4 FragColor;

void main() {
    vec3 diffuseColor = vec3(texture(material.baseColor, geometryInput.TexCoords));

    float gamma = 2.2;
    diffuseColor = pow(diffuseColor, vec3(1.0/gamma));

    FragColor = vec4(diffuseColor, 1.0);
}
