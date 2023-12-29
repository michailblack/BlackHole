#version 460 core

layout (location = 0) out vec3 o_Position;
layout (location = 1) out vec3 o_Normal;
layout (location = 2) out vec4 o_Albedo;

layout (location = 0) in VS_OUT
{
	vec2 TexCoord;
	vec3 FragmentPosition;
	mat3 TBN;
} fs_in;

struct Material
{
	sampler2DArray Diffuse;
	uint DiffuseLayer;

	sampler2DArray Specular;
	uint SpecularLayer;

	sampler2DArray Normal;
	uint NormalLayer;

	float Shininess;
};

uniform Material u_Material;

void main()
{
	o_Position = fs_in.FragmentPosition;

	o_Normal = texture(u_Material.Normal, vec3(fs_in.TexCoord, u_Material.NormalLayer)).rgb;
	o_Normal = o_Normal * 2.0 - 1.0;
	o_Normal = normalize(fs_in.TBN * o_Normal);

	o_Albedo.rgb = texture(u_Material.Diffuse, vec3(fs_in.TexCoord, u_Material.DiffuseLayer)).rgb;
	o_Albedo.a = texture(u_Material.Specular, vec3(fs_in.TexCoord, u_Material.SpecularLayer)).b;
}