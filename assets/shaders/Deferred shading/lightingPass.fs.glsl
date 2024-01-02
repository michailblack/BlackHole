#version 460 core

layout (location = 0) out vec4 o_Color;

layout (location = 0) in VS_OUT
{
	vec2 TexCoord;
	vec3 FragmentPosition;
} fs_in;

layout (std140, binding = 0) uniform Matrices
{
	mat4 u_Projection;
	mat4 u_View;
	mat4 u_InverseProjection;
	mat4 u_InverseView;
};

struct PointLight
{
	vec3 Position;
	vec3 Color;
	
	float Linear;
	float Quadratic;

	float VolumeRadius;
};

uniform sampler2D u_Position;
uniform sampler2D u_Normal;
uniform sampler2D u_AlbedoSpec;
uniform PointLight u_PointLight;

void main()
{
	vec3 fragmentPos = texture(u_Position  , fs_in.TexCoord).rgb;
	vec3 normal      = texture(u_Normal    , fs_in.TexCoord).rgb;
	vec3 albedo      = texture(u_AlbedoSpec, fs_in.TexCoord).rgb;

	if (normal == vec3(0.0))
		discard;
	
	vec3 lightPosition = (u_View * vec4(u_PointLight.Position, 1.0)).xyz;

	if (length(fragmentPos - lightPosition) > u_PointLight.VolumeRadius)
		discard;
	
	vec3 fragmentColor = vec3(0.0);
	
	vec3 lightDir = normalize(lightPosition - fragmentPos);
		
	// diffuse color
	vec3 diffuse = max(dot(normal, lightDir), 0.0) * albedo * u_PointLight.Color;
	
	// attenuation
    float dist = length(lightPosition - fragmentPos);
    float attenuation = 1.0 / (1.0 + u_PointLight.Linear * dist + u_PointLight.Quadratic * dist * dist);
	
	fragmentColor += diffuse * attenuation;

	o_Color = vec4(fragmentColor, 1.0);
}