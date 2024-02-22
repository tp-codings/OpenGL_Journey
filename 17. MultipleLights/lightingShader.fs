#version 330

in vec2 TexCoords;
in vec3 Normal;				//wird interpoliert -> Bekommen für jedes Fragment die Normale
in vec3 FragPos;

out vec4 FragColor; 

struct Material {

	sampler2D diffuse;			//Surfacecolor unter diffuse lighting
	sampler2D specular;			//Color von specular Highlight
	sampler2D emission;
	float shininess;
};


uniform vec3 viewPos;

uniform Material material;

uniform float time; 



//Directional light
struct DirLight 
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform DirLight dirLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);


//Point light
struct PointLight
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


//spotLight
struct SpotLight
{
	float cutOff;
	float outerCutOff;

	vec3 position;
	vec3 sourceDirection;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform SpotLight spotLight;

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);



void main()
{

	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	//phase 1: Directional lighting
	vec3 result = CalcDirLight(dirLight, norm, viewDir);

	//phase 2: Point lights
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	}

	// phase 3: Spot light 
	result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

	FragColor = vec4(result, 1.0);
}



vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	//diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	//specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	//results
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	//diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	//specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	//results
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	//attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.sourceDirection)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);

}