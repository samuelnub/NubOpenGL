#version 330 core

//declare structs here
struct Material
{
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;

void main()
{
	//ambient strength
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	//normalise normals to get direct pointing directions, and get light dir by subtracting
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);

	//calc diffuse impact
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	//specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	//if its greater than 90, there wont be a visible diffuse light (duh, you learned physics)
	vec3 result = ambient + diffuse + specular;
	color = vec4(result, 1.0f);
}