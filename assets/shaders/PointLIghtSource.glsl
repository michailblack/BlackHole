#type vertex
#version 460 core

out gl_PerVertex
{
	vec4 gl_Position;
};

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;
layout (location = 3) in vec3 a_Tangent;

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
}

#type fragment
#version 460 core

layout (location = 1) out vec4 o_Color;

uniform vec3 u_LightSourceColor;

void main()
{
	o_Color = vec4(u_LightSourceColor, 1.0);
}
