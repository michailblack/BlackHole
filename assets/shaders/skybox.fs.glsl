#version 460

layout (location = 2) out vec4 o_Color;

in vec3 v_SkyboxCoord;

uniform samplerCube u_Skybox;

void main()
{
    o_Color = texture(u_Skybox, v_SkyboxCoord);
}
