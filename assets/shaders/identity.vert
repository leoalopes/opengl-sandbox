#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoords;

out GeometryData {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
} geometryOutput;

void main() {
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); 
    geometryOutput.TexCoords = aTexCoords;
}
