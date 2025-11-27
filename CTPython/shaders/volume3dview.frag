#version 330 core

in vec3 TexCoord;
out vec4 FragColor;

uniform sampler3D volumeTex;

uniform mat4 invMVP;
uniform vec3 cameraPos;
uniform float minVal;
uniform float maxVal;

uniform int windowWidth;
uniform int windowHeight;

const float stepSize = 0.01;
const int maxSteps = 512;

uniform vec3 cubeWorld;

// These constants define the full world-space extents of your cube geometry.
// They are derived from your C++ cube scaling: 2.0 * (100 * 0.076), etc.
//const vec3 cubeWorld = vec3(
//    2.0 * (100.0 * 0.076),
//    2.0 * (100.0 * 0.219),
//    2.0 * (100.0 * 0.5)
//);

void main()
{
    // Reconstruct fragment position in clip space
    vec2 ndcXY = (gl_FragCoord.xy / vec2(windowWidth, windowHeight)) * 2.0 - 1.0;
    float clipZ = gl_FragCoord.z * 2.0 - 1.0;
    vec4 clip = vec4(ndcXY, clipZ, 1.0);

    // Convert to world space (ray origin on cube surface)
    vec4 worldPos_hom = invMVP * clip;
    vec3 worldPos = worldPos_hom.xyz / worldPos_hom.w;

    // Compute ray direction from camera to fragment in world space
    vec3 rayDir_world = normalize(worldPos - cameraPos);

    // Transform world-space ray direction into texture-space units
    // This accounts for the cube's non-uniform scaling in world space,
    // making the raymarch steps proportional in texture space.
    vec3 rayDir_texture = normalize(rayDir_world / cubeWorld);

    // Set starting point in texture space
    vec3 pos = TexCoord;

    float accumulated = 0.0;

    for (int i = 0; i < maxSteps; ++i)
    {
        // Stop if ray leaves the texture volume
        if (any(lessThan(pos, vec3(0.0))) || any(greaterThan(pos, vec3(1.0))))
            break;

        float sample = texture(volumeTex, pos).r;
        float normVal = clamp((sample - minVal) / (maxVal - minVal), 0.0, 1.0);

        float alpha = normVal * stepSize * 5.0;
        accumulated += alpha * (1.0 - accumulated);

        if (accumulated >= 0.95)
            break;

        // Raymarch in texture space with the corrected direction
        pos += rayDir_texture * stepSize;
    }

    FragColor = vec4(vec3(accumulated), accumulated);
}