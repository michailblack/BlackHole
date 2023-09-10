#version 460 core

layout (location = 0) out vec4 o_Color;
layout (location = 1) out vec4 o_BrightnessColor;

layout (location = 0) in VS_OUT
{
	vec3 FragmentPosition;
	vec2 TexCoord;
	vec4 FragmentPositionLightSpace;
	vec3 TangentCameraPosition;
	vec3 TangentLightDirection;
	vec3 TangentFragmentPosition;
} fs_in;

struct Material
{
	sampler2DArray Diffuse;
	uint DiffuseLayer;

	sampler2DArray Specular;
	uint SpecularLayer;

	sampler2DArray Normal;
	uint NormalLayer;

	sampler2DArray Displacement;
	uint DisplacementLayer;

	float Shininess;
};

struct DirectionalLight
{
	vec3 Direction;

	vec3 Diffuse;
	vec3 Specular;
};

uniform vec3 u_Color;
uniform Material u_Material;
uniform DirectionalLight u_DirectionalLight;
uniform sampler2D u_ShadowMap;
uniform samplerCube u_Skybox;

vec2 ParallaxMapping(in const vec2 texCoord, in const vec3 viewDir)
{
	const float minLayers = 8.0;
	const float maxLayers = 32.0;
	const float layersCount = mix(maxLayers, minLayers, max(dot(vec3(0.0, 0.0, 1.0), viewDir), 0.0));

    float layerDepth = 1.0 / layersCount;
    float currentLayerDepth = 0.0;

    vec2 P = viewDir.xy * 0.1; 
    vec2 deltaTexCoords = P / layersCount;

	vec2  currentTexCoord      = texCoord;
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

float CalculateShadow(in const vec4 fragmentPositionLightSpace, in const vec3 normal, in const vec3 lightDir)
{
	// perspective division -> [-1; 1]
	vec3 fragmentPositionLightSpaceNDC = fragmentPositionLightSpace.xyz / fragmentPositionLightSpace.w;

	// [-1; 1] -> [0; 1]
	vec3 projCoords = fragmentPositionLightSpaceNDC * 0.5 + 0.5;

	float currentDepth = projCoords.z;

	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(u_ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
		}    
	}
	shadow /= 9.0;

	if (currentDepth > 1.0)
		shadow = 0.0;
	
	return shadow;
}

vec4 BlinnPhongModel(in const vec3 normal, in const vec3 viewDir, in const vec3 lightDir, in const float shadow, in const vec2 texCoord)
{
	vec4 fragmentColor = texture(u_Material.Diffuse, vec3(texCoord, u_Material.DiffuseLayer));
	vec4 specularColor = texture(u_Material.Specular, vec3(texCoord, u_Material.SpecularLayer));

	vec3 ambinetStrength = vec3(0.1);
	vec3 ambient = ambinetStrength * fragmentColor.rgb;

	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = diff * u_DirectionalLight.Diffuse * fragmentColor.rgb;

	vec3 halfwayDirection = normalize(viewDir + lightDir);
	float spec = pow(max(dot(normal, halfwayDirection), 0.0), u_Material.Shininess);
	vec3 specular = spec * u_DirectionalLight.Specular * specularColor.rgb;

	return vec4((ambient + (1.0 - shadow) * (diffuse + specular)), fragmentColor.a);
}

void main()
{
	vec4 outputColor = vec4(0.0, 0.0, 0.0, 1.0);
	if (u_Color == vec3(0.0))
	{
		vec3 viewDir = normalize(fs_in.TangentCameraPosition - fs_in.TangentFragmentPosition);
		//vec2 texCoord = ParallaxMapping(fs_in.TexCoord, viewDir);
		vec2 texCoord = fs_in.TexCoord;

		if(texCoord.x > 1.0 || texCoord.y > 1.0 || texCoord.x < 0.0 || texCoord.y < 0.0)
			discard;

		vec3 normal = texture(u_Material.Normal, vec3(texCoord, u_Material.NormalLayer)).rgb;
		normal = normalize(normal * 2.0 - 1.0);

		vec3 directionalLightDir = normalize(-fs_in.TangentLightDirection);

		float directionalLightShadow = CalculateShadow(fs_in.FragmentPositionLightSpace, normal, directionalLightDir);
		outputColor = BlinnPhongModel(normal, viewDir, directionalLightDir, directionalLightShadow, texCoord);
	}
	else
	{
		outputColor = vec4(u_Color, 1.0);
	}

	o_Color = outputColor;

	float brightness = dot(outputColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0)
        o_BrightnessColor = vec4(outputColor.rgb, 1.0);
    else
        o_BrightnessColor = vec4(0.0, 0.0, 0.0, 1.0);
}