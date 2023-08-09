#version 460 core
layout (location = 0) out vec4 o_Color;

layout (location = 0) in VS_OUT
{
	vec3 FragmentPosition;
	vec3 Normal;
	vec2 TexCoord;
} fs_in;

layout (std140, binding = 0) uniform Matrices
{
	mat4 u_Projection;
	mat4 u_View;
};

struct Material
{
	sampler2DArray Diffuse;
	uint DiffuseLayer;

	sampler2DArray Specular;
	uint SpecularLayer;

	float Shininess;
};

struct DirectionalLight
{
	vec3 Direction;

	vec3 Diffuse;
	vec3 Specular;
};

uniform Material u_Material;
uniform DirectionalLight u_DirectionalLight;
uniform samplerCube u_Skybox;

vec4 BlinnPhongModel(in const vec3 normal, in const vec3 viewDir, in const vec3 lightDir)
{
	vec4 fragmentColor = texture(u_Material.Diffuse, vec3(fs_in.TexCoord, u_Material.DiffuseLayer));
	vec4 specularColor = texture(u_Material.Specular, vec3(fs_in.TexCoord, u_Material.SpecularLayer));

	float ambinetStrength = 0.1;
	vec4 ambient = vec4(ambinetStrength * u_DirectionalLight.Diffuse, 1.0) * fragmentColor;

	float diff = max(dot(normal, lightDir), 0.0);
	vec4 diffuse = vec4(diff * u_DirectionalLight.Diffuse, 1.0) * fragmentColor;

	vec3 halfwayDirection = normalize(viewDir + lightDir);
	float spec = pow(max(dot(normal, halfwayDirection), 0.0), u_Material.Shininess);
	vec4 specular = vec4(spec * u_DirectionalLight.Specular, 1.0) * specularColor;
	
	return (ambient + diffuse + specular);
}

void main()
{
	vec3 normal = normalize(fs_in.Normal);
	vec3 viewDir = normalize(-fs_in.FragmentPosition);

	if (dot(viewDir, normal) < 0)
	{
		discard;
	}

	vec3 directionalLightDir = -normalize(u_View * vec4(u_DirectionalLight.Direction, 1.0)).xyz;
	vec4 outputColor = BlinnPhongModel(normal, viewDir, directionalLightDir);

	o_Color = outputColor;
}