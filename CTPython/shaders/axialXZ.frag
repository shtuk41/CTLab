#version 330 core

uniform sampler3D volumeTex;
uniform float minVal;
uniform float maxVal;
uniform float ySlice;

in vec2 vTexCoord;
out vec4 FragColor;

void main()
{
    vec3 texCoord3D = vec3(vTexCoord.x, ySlice, vTexCoord.y);
    float sample = texture(volumeTex, texCoord3D).r;
    float normVal = clamp((sample - minVal) / (maxVal - minVal), 0.0, 1.0);

    FragColor = vec4(vec3(normVal), 1.0);
}