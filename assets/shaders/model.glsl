//////////////////////////////////////////////////////////////////////
#type vertex
#version 460 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TextureCoords;

out VS_OUT
{
	vec3 v_FragPos;
	vec3 v_Normal;
	vec2 v_TextureCoords;
} vs_out;

layout (std140, binding = 0) uniform Matrices
{
	mat4 u_Projection;
	mat4 u_View;
};
uniform mat4 u_Model;

void main()
{
	vs_out.v_FragPos = vec3(u_View * u_Model * vec4(a_Pos, 1.0));
	vs_out.v_Normal = normalize(mat3(transpose(inverse(u_View * u_Model))) * a_Normal);
	vs_out.v_TextureCoords = a_TextureCoords;
	
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0);
}

//////////////////////////////////////////////////////////////////////
#type fragment
#version 460 core
out vec4 FragColor;

struct Material
{
	sampler2D diffuse[32];
	sampler2D specular[32];
	float shininess;
};

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in VS_OUT
{
	vec3 v_FragPos;
	vec3 v_Normal;
	vec2 v_TextureCoords;
} fs_in;

uniform Material u_Material;
uniform DirLight dirLight;

uniform samplerCube u_Skybox;
uniform float u_DiffuseMapsUsed;
uniform float u_SpecularMapsUsed;

vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

void main()
{
	// properties
	vec3 norm = normalize(fs_in.v_Normal);
	vec3 viewDir = normalize(-fs_in.v_FragPos);
	
	vec4 result = CalcDirLight(dirLight, norm, viewDir);

	//vec3 R = refract(-viewDir, norm, 1.0 / 2.42);
	//vec3 R = reflect(-viewDir, norm);

	//result = vec4(texture(u_Skybox, R).rgb, 1.0);

	FragColor = result;
}

vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);

	// combine results
	vec4 ambient  = vec4(0.0);
	vec4 diffuse  = vec4(0.0);
	vec4 specular = vec4(0.0);
	for (int i = 0; i < u_DiffuseMapsUsed; ++i)
	{
		ambient  += vec4(light.ambient, 1.0)  *        texture(u_Material.diffuse[i],  fs_in.v_TextureCoords);
		diffuse  += vec4(light.diffuse, 1.0)  * diff * texture(u_Material.diffuse[i],  fs_in.v_TextureCoords);
	}
	for (int i = 0; i < u_SpecularMapsUsed; ++i)
	{
		specular += vec4(light.specular, 1.0) * spec * texture(u_Material.specular[i], fs_in.v_TextureCoords);
	}

	return (ambient + diffuse + specular);
}