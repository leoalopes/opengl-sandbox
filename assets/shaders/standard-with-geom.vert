#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 normalMatrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VertexData {
    vec3 Position;
    vec3 OriginalNormal;
    vec3 Normal;
    vec2 TexCoords;
} vertexOutput;

void main() {
    gl_Position = vec4(aPos, 1.0);
    vertexOutput.Position = vec3(model * vec4(aPos, 1.0));
    vertexOutput.OriginalNormal = aNormal;
    vertexOutput.Normal = mat3(normalMatrix) * aNormal;
    vertexOutput.TexCoords = aTexCoords;
}
