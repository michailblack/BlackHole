//////////////////////////////////////////////////////////////////////
#type vertex
#version 460 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;

out VS_OUT
{
	vec3 v_Normal;
} vs_out;

layout (std140, binding = 0) uniform Matrices
{
	mat4 u_Projection;
	mat4 u_View;
};
uniform mat4 u_Model;

void main()
{
	vs_out.v_Normal = normalize(mat3(transpose(inverse(u_View * u_Model))) * a_Normal);
	gl_Position = u_View * u_Model * vec4(a_Pos, 1.0);
}

//////////////////////////////////////////////////////////////////////
#type geometry
#version 460 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

layout (std140, binding = 0) uniform Matrices
{
	mat4 u_Projection;
	mat4 u_View;
};

in VS_OUT
{
	vec3 v_Normal;
} gs_in[];

void GenerateLine(int index)
{
	gl_Position = u_Projection * gl_in[index].gl_Position;
	EmitVertex();
	gl_Position = u_Projection * (gl_in[index].gl_Position + vec4(gs_in[index].v_Normal, 0.0) * 0.05);
	EmitVertex();
	EndPrimitive();
}

void main()
{
	GenerateLine(0);
	GenerateLine(1);
	GenerateLine(2);
}

//////////////////////////////////////////////////////////////////////
#type fragment
#version 460 core
out vec4 FragColor;

void main()
{
	FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}