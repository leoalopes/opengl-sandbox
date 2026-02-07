#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VertexData {
    vec3 Position;
    vec3 OriginalNormal;
    vec3 Normal;
    vec2 TexCoords;
} vertexInput[];

out GeometryData {
    vec3 Position;
    vec3 Normal;
    vec2 TexCoords;
} geometryOutput;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

vec4 explode(vec4 position, vec3 normal) {
    float speed = 5.0f;
    float magnitude = 0.05;
    vec3 direction = normal * ((sin(time * speed) + 1.0) / 2.0) * magnitude; 
    return position + vec4(direction, 0.0);
}

void main() {
    gl_Position = projection * view * model * explode(gl_in[0].gl_Position, vertexInput[0].OriginalNormal); 
    geometryOutput.Position = vertexInput[0].Position;
    geometryOutput.Normal = vertexInput[0].Normal;
    geometryOutput.TexCoords = vertexInput[0].TexCoords;
    EmitVertex();

    gl_Position = projection * view * model * explode(gl_in[1].gl_Position, vertexInput[1].OriginalNormal); 
    geometryOutput.Position = vertexInput[1].Position;
    geometryOutput.Normal = vertexInput[1].Normal;
    geometryOutput.TexCoords = vertexInput[1].TexCoords;
    EmitVertex();

    gl_Position = projection * view * model * explode(gl_in[2].gl_Position, vertexInput[2].OriginalNormal); 
    geometryOutput.Position = vertexInput[2].Position;
    geometryOutput.Normal = vertexInput[2].Normal;
    geometryOutput.TexCoords = vertexInput[2].TexCoords;
    EmitVertex();

    EndPrimitive();
} 
