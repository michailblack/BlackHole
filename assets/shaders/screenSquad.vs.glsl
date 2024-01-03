#version 460 core

out gl_PerVertex
{
	vec4 gl_Position;
};

layout (location = 0) in vec2 a_Position;
layout (location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;
out vec3 v_SkyboxCoord;

layout (std140, binding = 0) uniform Matrices
{
	mat4 u_Projection;
	mat4 u_View;
	mat4 u_InverseProjection;
	mat4 u_InverseView;
};

void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = vec4(a_Position, 0.0, 1.0);
	v_SkyboxCoord = mat3(u_InverseView) * (u_InverseProjection * gl_Position).xyz;
}