#type vertex
#version 460 core
layout (location = 0) in vec3 a_Pos;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0);
}

#type fragment
#version 460 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(0.18, 0.32, 0.32, 1.0);
}  