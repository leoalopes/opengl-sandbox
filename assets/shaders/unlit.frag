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
    FragColor = vec4(diffuseColor, 1.0);
}
