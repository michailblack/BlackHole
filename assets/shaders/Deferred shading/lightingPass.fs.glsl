#version 460 core

layout (location = 0) out vec4 o_Color;

in vec2 v_TexCoord;

const int MAX_POINT_LIGHTS_COUNT = 1;

struct PointLight
{
	vec3 Position;
	vec3 Color;

	float Linear;
	float Quadratic;
};

layout (std140, binding = 0) uniform Matrices
{
	mat4 u_Projection;
	mat4 u_View;
};

layout (std140, binding = 1) uniform PointLigths
{
	PointLight PointLights[MAX_POINT_LIGHTS_COUNT];
};

uniform sampler2D u_Position;
uniform sampler2D u_Normal;
uniform sampler2D u_AlbedoSpec;

void main()
{
	vec3 fragmentPos     = texture(u_Position  , v_TexCoord).rgb;
	vec3 normal          = texture(u_Normal    , v_TexCoord).rgb;
	vec3 albedo          = texture(u_AlbedoSpec, v_TexCoord).rgb;

	vec3 viewDir = normalize(-fragmentPos);

	// ambient color
	vec3 fragmentColor = albedo * 0.1;
	
	for (uint i = 0; i < MAX_POINT_LIGHTS_COUNT; ++i)
	{
		vec3 lightPosition = (u_View * vec4(PointLights[i].Position, 1.0)).xyz;
		vec3 lightDir = normalize(lightPosition - fragmentPos);
		
		// diffuse color
		vec3 diffuse = max(dot(normal, lightDir), 0.0) * albedo * PointLights[i].Color;

		// attenuation
        float dist = length(lightPosition - fragmentPos);
        float attenuation = 1.0 / (1.0 + PointLights[i].Linear * dist + PointLights[i].Quadratic * dist * dist);

		fragmentColor += diffuse * attenuation;
	}

	float exposure = 1.0;
	fragmentColor = vec3(1.0) - exp(-fragmentColor * exposure);

	float gamma = 2.2;
	fragmentColor = pow(fragmentColor, vec3(1.0 / gamma));

	o_Color = vec4(fragmentColor, 1.0);
}