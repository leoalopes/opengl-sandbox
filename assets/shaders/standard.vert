#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 aTangent;
layout (location = 3) in vec2 aTexCoords;

uniform mat4 normalMatrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out GeometryData {
    vec3 Position;
    vec3 Normal;
    mat3 TBNMatrix;
    vec2 TexCoords;
} geometryOutput;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    geometryOutput.Position = vec3(model * vec4(aPos, 1.0));

    geometryOutput.Normal = normalize(mat3(normalMatrix) * aNormal);
    vec3 tangent = normalize(mat3(normalMatrix) * aTangent.xyz);
    vec3 bitangent = cross(geometryOutput.Normal, tangent);
    geometryOutput.TBNMatrix = mat3(tangent, bitangent, geometryOutput.Normal);

    geometryOutput.TexCoords = aTexCoords;
}
