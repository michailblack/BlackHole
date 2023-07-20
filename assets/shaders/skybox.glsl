#type vertex
#version 460 core
layout (location = 0) in vec3 a_Pos;

out vec3 v_TexCoords;

uniform mat4 u_Projection;
uniform mat4 u_View;

void main()
{
	v_TexCoords = a_Pos;
	gl_Position = (u_Projection * u_View * vec4(a_Pos, 1.0)).xyww;
}

#type fragment
#version 460 core
out vec4 FragColor;

in vec3 v_TexCoords;

uniform samplerCube u_Skybox;

void main()
{
    FragColor = texture(u_Skybox, v_TexCoords);
}  