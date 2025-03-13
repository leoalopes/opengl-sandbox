#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 normalMatrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float extraOffset;

void main() {
    vec3 normal = normalize(mat3(normalMatrix) * aNormal);
    vec3 offset = extraOffset * normal;
    vec3 position = vec3(model * vec4(aPos, 1.0));
    position += offset;

    gl_Position = projection * view * vec4(position, 1.0);
}
