#version 460 core

out gl_PerVertex
{
	vec4 gl_Position;
};

layout (location = 0) in vec3 a_Position;

uniform mat4 u_Model;
uniform mat4 u_LightSpaceTransform;

void main()
{
	gl_Position = u_LightSpaceTransform * u_Model * vec4(a_Position, 1.0);
}