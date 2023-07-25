#type vertex
#version 460 core
layout (location = 0) in vec3 a_Pos;

out vec3 v_TexCoords;

layout (std140, binding = 0) uniform Matrices
{
	mat4 u_Projection;
	mat4 u_View;
};

uniform mat4 u_CameraRotation;

void main()
{
	v_TexCoords = a_Pos;
	gl_Position = (u_Projection * u_CameraRotation * vec4(a_Pos, 1.0)).xyww;
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