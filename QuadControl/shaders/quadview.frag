#version 330 core
uniform vec3 baseColor;
out vec4 fragColor;
void main() {
    fragColor = vec4(baseColor, 1.0);
}
