#version 460 core

layout (location = 0) out vec4 o_Color;

in vec2 v_TexCoord;

layout (std140, binding = 0) uniform Matrices
{
	mat4 u_Projection;
	mat4 u_View;
	mat4 u_InverseProjection;
	mat4 u_InverseView;
};

uniform sampler2D u_Scene;

void main()
{
	vec3 fragmentColor = texture(u_Scene, v_TexCoord).rgb;
	
	float exposure = 1.0;
	fragmentColor = vec3(1.0) - exp(-fragmentColor * exposure);
	
	float gamma = 2.2;
	fragmentColor = pow(fragmentColor, vec3(1.0 / gamma));

	o_Color = vec4(fragmentColor, 1.0);
}