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

uniform int screenHalf;

out vec4 FragColor;

void main() {
    bool inMiddle = abs(gl_FragCoord.x - screenHalf) <= 3;
    if (inMiddle) {
        FragColor = vec4(0.3, 0.3, 0.3, 1.0);
        return;
    }

    vec3 diffuseColor = vec3(texture(material.baseColor, geometryInput.TexCoords));

    float gamma = 2.2;
    diffuseColor = pow(diffuseColor, vec3(1.0/gamma));

    FragColor = vec4(diffuseColor, 1.0);
}
