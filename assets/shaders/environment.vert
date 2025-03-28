#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main() {
    vec4 position = vec4(aPos, 1.0);
    TexCoords = (inverse(view) * inverse(projection) * position).xyz;
    gl_Position = position;
}
