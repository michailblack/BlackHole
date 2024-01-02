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

	sampler2DArray Normal;
	uint NormalLayer;

	sampler2DArray Displacement;
	uint DisplacementLayer;
};

uniform Material u_Material;

vec2 ParallaxMapping()
{
	const vec3 viewDir = normalize(-fs_in.FragmentPosition);

	const float minLayers = 8.0;
	const float maxLayers = 32.0;
	const float layersCount = mix(maxLayers, minLayers, max(dot(vec3(0.0, 0.0, 1.0), viewDir), 0.0));

    float layerDepth = 1.0 / layersCount;
    float currentLayerDepth = 0.0;

    vec2 P = viewDir.xy * 0.1; 
    vec2 deltaTexCoords = P / layersCount;

	vec2  currentTexCoord      = fs_in.TexCoord;
	float currentDepthMapValue = texture(u_Material.Displacement, vec3(currentTexCoord, u_Material.DisplacementLayer)).r;

	while(currentLayerDepth < currentDepthMapValue)
	{
		currentTexCoord -= deltaTexCoords;
		currentDepthMapValue = texture(u_Material.Displacement, vec3(currentTexCoord, u_Material.DisplacementLayer)).r;
		currentLayerDepth += layerDepth;
	}

	vec2 prevTexCoord = currentTexCoord + deltaTexCoords;

	// get depth after and before collision for linear interpolation
	float afterDepth  = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = texture(u_Material.Displacement, vec3(prevTexCoord, u_Material.DisplacementLayer)).r - currentLayerDepth + layerDepth;
 
	float weight = afterDepth / (afterDepth - beforeDepth);
	vec2 finalTexCoords = prevTexCoord * weight + currentTexCoord * (1.0 - weight);

	return finalTexCoords; 
}

void main()
{
	o_Position = fs_in.FragmentPosition;

	const vec2 texCoords = ParallaxMapping();

	o_Normal = texture(u_Material.Normal, vec3(texCoords, u_Material.NormalLayer)).rgb;
	o_Normal = o_Normal * 2.0 - 1.0;
	o_Normal = normalize(fs_in.TBN * o_Normal);

	o_Albedo.rgb = texture(u_Material.Diffuse, vec3(texCoords, u_Material.DiffuseLayer)).rgb;
}