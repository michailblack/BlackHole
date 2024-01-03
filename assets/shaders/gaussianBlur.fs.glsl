#version 460 core

layout (location = 0) out vec4 o_Color;

in vec2 v_TexCoord;
in vec3 v_SkyboxCoord;

uniform float u_ViewportWidth;
uniform float u_ViewportHeight;
uniform sampler2D u_Scene;

void main()
{
	float Pi = 6.28318530718; // Pi*2
    
    float directions = 16.0;
    float quality = 4.0;
    float size = 2.0;
   
    vec2 radius = size / vec2(u_ViewportWidth, u_ViewportHeight);
     
    vec4 fragmentColor = texture(u_Scene, v_TexCoord);
    
    // Blur calculations
    for (float d = 0.0; d < Pi; d += Pi / directions)
    {
        for (float i = 1.0 / quality; i <= 1.0; i += 1.0 / quality)
        {
			fragmentColor += texture(u_Scene, v_TexCoord + vec2(cos(d),sin(d)) * radius * i);		
        }
    }
    
    // Output to screen
    fragmentColor /= quality * directions - 15.0;

	o_Color = vec4(fragmentColor.rgb, 1.0);
}