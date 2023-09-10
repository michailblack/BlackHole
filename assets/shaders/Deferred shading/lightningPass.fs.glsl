#version 460 core

layout (location = 0) out vec4 o_Color;

in vec2 v_TexCoord;

const int MAX_POINT_LIGHTS_COUNT = 32;

struct PointLight
{
	vec3 Position;
	vec3 Color;

	float Linear;
	float Quadratic;
};

layout (std140, binding = 1) uniform PointLigths
{
	PointLight PointLights[MAX_POINT_LIGHTS_COUNT];
};

uniform sampler2D u_Position;
uniform sampler2D u_Normal;
uniform sampler2D u_AlbedoSpec;

uniform vec3 u_CameraPos;

void main()
{
	vec3 fragmentPos     = texture(u_Position  , v_TexCoord).rgb;
	vec3 normal          = texture(u_Normal    , v_TexCoord).rgb;
	vec3 albedo          = texture(u_AlbedoSpec, v_TexCoord).rgb;
	float specularScalar = texture(u_AlbedoSpec, v_TexCoord).a;

	if (normal == vec3(0.0))
		discard;

	vec3 viewDir = normalize(u_CameraPos - fragmentPos);

	// ambient color
	vec3 fragmentColor = albedo * 0.1;
	
	for (uint i = 0; i < MAX_POINT_LIGHTS_COUNT; ++i)
	{
		vec3 lightDir = normalize(PointLights[i].Position - fragmentPos);
		
		// diffuse color
		vec3 diffuse = max(dot(normal, lightDir), 0) * albedo * PointLights[i].Color;

		// specular color
		vec3 halfwayDir = normalize(viewDir + lightDir);
		float spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);
        vec3 specular = PointLights[i].Color * spec * specularScalar;

		// attenuation
        float dist = length(PointLights[i].Position - fragmentPos);
        float attenuation = 1.0 / (1.0 + PointLights[i].Linear * dist + PointLights[i].Quadratic * dist * dist);
        diffuse *= attenuation;
        specular *= attenuation;

		fragmentColor += diffuse + specular;
	}

	float exposure = 1.0;
	fragmentColor = vec3(1.0) - exp(-fragmentColor * exposure);

	float gamma = 2.2;
	fragmentColor = pow(fragmentColor, vec3(1.0 / gamma));

	o_Color = vec4(fragmentColor, 1.0);
}