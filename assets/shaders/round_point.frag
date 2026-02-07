#version 330 core

uniform vec3 color;

out vec4 FragColor;

void main() {
    vec2 center = vec2(0.5, 0.5);
    float dist = distance(gl_PointCoord, center);
    if (dist > 0.5) {
        discard;
    }

    float strength = 1.0 - (dist * 2.0); 
    FragColor = vec4(color, strength);
}
