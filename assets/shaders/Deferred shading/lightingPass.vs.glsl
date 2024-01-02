#version 460 core

out gl_PerVertex
{
	vec4 gl_Position;
};

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in vec3 a_Tangent;

layout (location = 0) out VS_OUT
{
	vec2 TexCoord;
	vec3 FragmentPosition;
} vs_out;

layout (std140, binding = 0) uniform Matrices
{
	mat4 u_Projection;
	mat4 u_View;
	mat4 u_InverseProjection;
	mat4 u_InverseView;
};

uniform mat4 u_Model;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
	
	// From clip-space [-1; 1] to UV coordinates [0; 1]
	vs_out.TexCoord = ((gl_Position.xy / gl_Position.w) + 1.0) / 2.0;

	vs_out.FragmentPosition = vec3(u_View * u_Model * vec4(a_Position, 1.0));
}
