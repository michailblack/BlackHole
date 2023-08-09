#type vertex
#version 460 core
layout (location = 0) in vec3 a_Position;

out vec3 v_TexCoords;

layout (std140, binding = 0) uniform Matrices
{
	mat4 u_Projection;
	mat4 u_View;
};

void main()
{
	v_TexCoords = a_Position;
	gl_Position = (u_Projection * mat4(mat3(u_View)) * vec4(a_Position, 1.0)).xyww;
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