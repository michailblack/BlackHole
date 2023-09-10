#version 460 core
layout (location = 0) out vec4 o_Color;

in vec2 v_TexCoord;

uniform sampler2D u_SceneImage;
uniform sampler2D u_BloomMap;

void main()
{
	const float gamma = 2.2;
    vec3 hdrColor = texture(u_SceneImage, v_TexCoord).rgb;      
    vec3 bloomColor = texture(u_BloomMap, v_TexCoord).rgb;
    hdrColor += bloomColor; // additive blending
    // tone mapping
    float exposure = 0.1;
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // also gamma correct while we're at it       
    result = pow(result, vec3(1.0 / gamma));
    o_Color = vec4(result, 1.0);
}