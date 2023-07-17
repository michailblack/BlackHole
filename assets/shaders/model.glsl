#type vertex
#version 460 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TextureCoords;

out vec3 v_FragPos;
out vec3 v_Normal;
out vec2 v_TextureCoords;

uniform mat4 u_Projection;
uniform mat4 u_View;
uniform mat4 u_Model;

void main()
{
	v_FragPos = vec3(u_View * u_Model * vec4(a_Pos, 1.0));
	v_Normal = mat3(transpose(inverse(u_View * u_Model))) * a_Normal;
	v_TextureCoords = a_TextureCoords;
	
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0);
}

#type fragment
#version 460 core
out vec4 FragColor;

struct Material
{
	sampler2D diffuse[8];
	sampler2D specular[8];
	float shininess;
};

struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

//struct PointLight
//{
//	vec3 position;
//	
//	float constant;
//	float linear;
//	float quadratic;
//
//	vec3 ambient;
//	vec3 diffuse;
//	vec3 specular;
//};
//
//struct SpotLight
//{
//    vec3 position;
//    vec3 direction;
//    float cutOff;
//    float outerCutOff;
//  
//    float constant;
//    float linear;
//    float quadratic;
//  
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//};

//#define NR_POINT_LIGHTS 4

in vec3 v_FragPos;
in vec3 v_Normal;
in vec2 v_TextureCoords;

uniform Material material;
uniform DirLight dirLight;
//uniform PointLight pointLights[NR_POINT_LIGHTS];
//uniform SpotLight spotLight;

uniform int u_DiffuseMapsUsed;
uniform int u_SpecularMapsUsed;

vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
//vec3 CalcPointLight(PointLight light, vec3 normal, vec3 v_FragPos, vec3 viewDir);
//vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 v_FragPos, vec3 viewDir);

void main()
{
	// properties
	vec3 norm = normalize(v_Normal);
	vec3 viewDir = normalize(-v_FragPos);
	
	// phase 1: Directional lighting
	vec4 result = CalcDirLight(dirLight, norm, viewDir);
	
	//// phase 2: Point lights
	//for (int i = 0; i < NR_POINT_LIGHTS; ++i)
	//	result += CalcPointLight(pointLights[i], norm, v_FragPos, viewDir);
	//
	//// phase 3: Spot light
	//result += CalcSpotLight(spotLight, norm, v_FragPos, viewDir);
	
	FragColor = result;
}

vec4 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// combine results
	vec4 ambient  = vec4(0.0);
	vec4 diffuse  = vec4(0.0);
	vec4 specular = vec4(0.0);
	for (int i = 0; i < u_DiffuseMapsUsed; ++i)
	{
		ambient  += vec4(light.ambient, 1.0)  *        texture(material.diffuse[i],  v_TextureCoords);
		diffuse  += vec4(light.diffuse, 1.0)  * diff * texture(material.diffuse[i],  v_TextureCoords);
	}
	for (int i = 0; i < u_SpecularMapsUsed; ++i)
	{
		specular += vec4(light.specular, 1.0) * spec * texture(material.specular[i], v_TextureCoords);
	}

	return (ambient + diffuse + specular);
}

//vec3 CalcPointLight(PointLight light, vec3 normal, vec3 v_FragPos, vec3 viewDir)
//{
//	vec3 lightDir = normalize(light.position - v_FragPos);
//
//	// diffuse shading
//	float diff = max(dot(normal, lightDir), 0.0);
//
//	// specular shading
//	vec3 reflectDir = reflect(-lightDir, normal);
//	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//
//	// attenuation
//	float lightDistance = length(light.position - v_FragPos);
//	float attenuation = 1.0 / (light.constant + light.linear * lightDistance + light.quadratic * (lightDistance * lightDistance));
//
//	// combine results
//	vec3 ambient = light.ambient * vec3(texture(material.diffuse, v_TextureCoords));
//	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, v_TextureCoords));
//	vec3 specular = light.specular * spec * vec3(texture(material.specular, v_TextureCoords));
//
//	ambient *= attenuation;
//	diffuse *= attenuation;
//	specular *= attenuation;
//
//	return (ambient + diffuse + specular);
//};
//
//vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 v_FragPos, vec3 viewDir)
//{
//	vec3 lightDir = normalize(light.position - v_FragPos);
//
//	float theta = dot(lightDir, normalize(-light.direction));
//	float epsilon = light.cutOff - light.outerCutOff;
//	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
//
//	// diffuse shading
//	float diff = max(dot(normal, lightDir), 0.0);
//
//	// specular shading
//	vec3 reflectDir = reflect(-lightDir, normal);
//	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//	
//	// attenuation
//	float lightDistance = length(light.position - v_FragPos);
//	float attenuation = 1.0 / (light.constant + light.linear * lightDistance + light.quadratic * (lightDistance * lightDistance));
//
//	// combine results
//	vec3 ambient = light.ambient * vec3(texture(material.diffuse, v_TextureCoords));
//	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, v_TextureCoords));
//	vec3 specular = light.specular * spec * vec3(texture(material.specular, v_TextureCoords));
//
//	ambient *= intensity * attenuation;
//	diffuse *= intensity * attenuation;
//	specular *= intensity * attenuation;
//
//	return (ambient + diffuse + specular);
//};