#type vertex
#version 460 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_TextureCoords;

out vec2 v_TextureCoords;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
	v_TextureCoords = a_TextureCoords;
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0);
}

#type fragment
#version 460 core
out vec4 FragColor;

in vec2 v_TextureCoords;

uniform sampler2D u_Texture;

void main()
{
	FragColor = texture(u_Texture, v_TextureCoords);
}