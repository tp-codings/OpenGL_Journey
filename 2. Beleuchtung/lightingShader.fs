#version 330 core


struct Material {
	vec3 ambient;			//Colorreflection bei Ambient light (Surfacecolor)
	vec3 diffuse;			//Surfacecolor unter diffuse lighting
	vec3 specular;			//Color von specular Highlight
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

};



in vec3 ourColor; 
in vec2 TexCoord;
in vec3 Normal;				//wird interpoliert -> Bekommen für jedes Fragment die Normale
in vec3 FragPos;

out vec4 FragColor; 

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 viewPos;

uniform Material material;
uniform Light light;

float spec; 
uniform bool blinn;
void main()
{
	//ambient light
	vec3 ambient = material.ambient * light.ambient;

	//diffuse light
	vec3 lightDirection = normalize(light.position - FragPos);

	float diff = max(dot(Normal, lightDirection), 0.0f);		//max, um positiven Wert zu bekommen
	vec3 diffuse = diff * material.diffuse * light.diffuse;

	//specular light
	vec3 viewDirection = normalize(viewPos - FragPos);
	vec3 reflectDirection = reflect(-lightDirection, Normal);

	if(blinn)
	{
		vec3 halfwayDir = normalize(lightDirection + viewDirection);
		spec = pow(max(dot(Normal, halfwayDir), 0.0), 256.0);
	}
	else
	{
		spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), material.shininess);	//32 ist shininess-level
	}


	vec3 specular = spec * material.specular * light.specular;
	//resulting light
	vec3 result = ambient + diffuse + specular;

	FragColor = vec4(result, 1.0f);
} 