#version 460 core

layout (location = 1) out vec4 o_Color;

in vec2 v_TexCoord;
in vec3 v_SkyboxCoord;

layout (std140, binding = 0) uniform Matrices
{
	mat4 u_Projection;
	mat4 u_View;
	mat4 u_InverseProjection;
	mat4 u_InverseView;
};

uniform sampler2D u_Position;
uniform sampler2D u_Normal;
uniform sampler2D u_AlbedoSpec;
uniform sampler2D u_LightMap;
uniform samplerCube u_Skybox;

void main()
{
	vec3 fragmentPos = texture(u_Position  , v_TexCoord).rgb;
	vec3 normal      = texture(u_Normal    , v_TexCoord).rgb;
	vec3 albedo      = texture(u_AlbedoSpec, v_TexCoord).rgb;
	vec3 lightMap    = texture(u_LightMap  , v_TexCoord).rgb;
	
	// ambient + diffuse
	vec3 fragmentColor = albedo * 0.01 + lightMap;

	if (normal == vec3(0.0))
		fragmentColor = texture(u_Skybox, v_SkyboxCoord).rgb;

	o_Color = vec4(fragmentColor, 1.0);
}