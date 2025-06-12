#version 330 core

struct Material {
    sampler2D baseColor;
};

in vec2 TexCoords;

uniform Material material;

out vec4 FragColor;

void main() {
    vec3 baseColor = vec3(texture(material.baseColor, TexCoords));
    FragColor = vec4(baseColor, 1.0);
}
