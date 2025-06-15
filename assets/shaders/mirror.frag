#version 330 core

struct Material {
    sampler2D mirror;
};

in vec2 TexCoords;

uniform Material material;

out vec4 FragColor;

void main() {
    vec3 diffuseColor = vec3(texture(material.mirror, TexCoords));
    FragColor = vec4(diffuseColor, 1.0);
}
