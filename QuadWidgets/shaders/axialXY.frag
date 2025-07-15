#version 330 core

uniform sampler3D volumeTex;
uniform int windowWidth;
uniform int windowHeight;

in vec4 colorOut;
out vec4 color_out;


void main()
{
	vec2 uv = gl_FragCoord.xy / vec2(windowWidth, windowHeight);
	float sliceZ = 0.5;

	vec3 pos = vec3(uv, sliceZ);
	
	float sample = texture(volumeTex, pos).r;
	color_out = vec4(vec3(sample), 1.0);
}