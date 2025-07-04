#version 330 core

in vec3 TexCoord;           // Interpolated texture-space position, assumed in [0,1]
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

void main()
{
    // 1. Reconstruct current fragment position in clip space
    vec2 ndcXY = (gl_FragCoord.xy / vec2(windowWidth, windowHeight)) * 2.0 - 1.0;
    float clipZ = gl_FragCoord.z * 2.0 - 1.0;
    vec4 clip = vec4(ndcXY, clipZ, 1.0);

    // 2. Convert to world space
    vec4 worldPos = invMVP * clip;
    worldPos /= worldPos.w;

    // 3. Compute ray direction from camera to fragment in world space
    vec3 rayDir = normalize(worldPos.xyz - cameraPos);

    // 4. Set starting point in texture space
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

        pos += rayDir * stepSize;  // Raymarch in texture space
    }

    FragColor = vec4(vec3(accumulated), accumulated);
}
