//////////////////////////////////////////////////////////////////////
#type vertex
#version 460 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

out VS_OUT
{
	vec3 FragPos;
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
	vs_out.FragPos = vec3(u_View * u_Model * vec4(a_Pos, 1.0));
	vs_out.Normal = normalize(mat3(transpose(inverse(u_View * u_Model))) * a_Normal);
	vs_out.TexCoord = a_TexCoord;
	
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0);
}

//////////////////////////////////////////////////////////////////////
#type fragment
#version 460 core
layout (location = 0) out vec4 o_Color;

struct Material
{
	sampler2DArray Diffuse;
	uint DiffuseLayer;
	sampler2DArray Specular;
	uint SpecularLayer;
	float Shininess;
};

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoord;
} fs_in;

uniform Material u_Material;

uniform samplerCube u_Skybox;

void main()
{
	vec3 norm = normalize(fs_in.Normal);
	vec3 viewDir = normalize(-fs_in.FragPos);

	if (dot(viewDir, norm) < 0)
	{
		discard;
	}

	//vec3 R = refract(-viewDir, norm, 1.0 / 2.42);
	//vec3 R = reflect(-viewDir, norm);

	//result = vec4(texture(u_Skybox, R).rgb, 1.0);

	o_Color = texture(u_Material.Diffuse, vec3(fs_in.TexCoord, u_Material.DiffuseLayer));
}