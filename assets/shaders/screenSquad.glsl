#type vertex
#version 460 core
layout (location = 0) in vec2 a_Pos;
layout (location = 1) in vec2 a_TextureCoords;

out vec2 v_TextureCoords;

void main()
{
	v_TextureCoords = a_TextureCoords;
	gl_Position = vec4(a_Pos.x, a_Pos.y, 0.0, 1.0);
}

#type fragment
#version 460 core
out vec4 FragColor;

in vec2 v_TextureCoords;

uniform sampler2D u_ScreenTexture;

const float offset = 1.0 / 300.0;  

void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0   ,  offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0),    // center-left
        vec2( 0.0   ,  0.0),    // center-center
        vec2( offset,  0.0),    // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0   , -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        1,  1,  1,
        1, -8,  1,
        1,  1,  1
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(u_ScreenTexture, v_TextureCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    FragColor = vec4(col, 1.0);
}  