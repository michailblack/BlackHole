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
	vec3 FragmentPosition;
	vec2 TexCoord;
	mat3 TBN;
} vs_out;

layout (std140, binding = 0) uniform Matrices
{
	mat4 u_Projection;
	mat4 u_View;
};

uniform mat4 u_Model;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Position, 1.0);
	
	mat4 normalMatrix = transpose(inverse(u_Model));

	vec3 T = normalize(vec3(normalMatrix * vec4(a_Tangent,   0.0)));
	vec3 N = normalize(vec3(normalMatrix * vec4(a_Normal,    0.0)));

	// re-orthogonalize T with respect to N
	T = normalize(T - dot(T, N) * N);
	// then retrieve perpendicular vector B with the cross product of T and N
	vec3 B = cross(N, T);

	vs_out.TBN = mat3(T, B, N);
	
	vs_out.FragmentPosition = vec3(u_Model * vec4(a_Position, 1.0));
	vs_out.TexCoord = a_TexCoord;
}
