#version 460 core

out gl_PerVertex
{
	vec4 gl_Position;
};

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

layout (location = 0) out VS_OUT
{
	vec3 FragmentPosition;
	vec3 Normal;
	vec2 TexCoord;
} vs_out;

layout (std140, binding = 0) uniform Matrices
{
	mat4 u_Projection;
	mat4 u_View;
};

uniform mat4 u_Model;

void main()
{
	vs_out.FragmentPosition = vec3(u_View * u_Model * vec4(a_Position, 1.0));
	vs_out.Normal = normalize(mat3(transpose(inverse(u_View * u_Model))) * a_Normal);
	vs_out.TexCoord = a_TexCoord;
	
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
}