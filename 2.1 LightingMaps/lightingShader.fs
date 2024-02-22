#version 330 core


struct Material {

	sampler2D diffuse;			//Surfacecolor unter diffuse lighting
	sampler2D specular;			//Color von specular Highlight
	sampler2D emission;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 directional;

	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outerCutOff;

};


in vec2 TexCoord;
in vec3 Normal;				//wird interpoliert -> Bekommen für jedes Fragment die Normale
in vec3 FragPos;

out vec4 FragColor; 

uniform vec3 viewPos;

uniform Material material;
uniform Light light;
uniform float time; 


void main()
{
	vec2 uv = TexCoord.xy;
	uv.x += time;
	vec3 color = vec3(1.0, 0.0, 0.0);

	//spotlight
	vec3 lightDirection = normalize(light.position - FragPos);
	float theta = dot(lightDirection, normalize(-light.directional));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff)/epsilon, 0.0, 1.0);

	
	//attenuation für point light
	float distance =  length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	//attenuation = 1.0;

	//ambient light
	vec3 ambient = vec3(texture(material.diffuse, TexCoord)) * light.ambient * attenuation;

	//directional light
	//lightDirection = normalize(-light.directional);

	//diffuse light
	float diff = max(dot(Normal, lightDirection), 0.0f);		//max, um positiven Wert zu bekommen
	vec3 diffuse = diff * vec3(texture(material.diffuse, TexCoord)) * light.diffuse * attenuation * intensity;

	//specular light
	vec3 viewDirection = normalize(viewPos - FragPos);
	vec3 reflectDirection = reflect(-lightDirection, Normal);

	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0f), material.shininess);	//32 ist shininess-level
	vec3 specular = spec * vec3(texture(material.specular, TexCoord)) * light.specular * attenuation * intensity;

	//emission
	vec3 emission = texture(material.emission,TexCoord + vec2(0.0,time)).rgb * floor(vec3(1.f) - texture(material.specular,TexCoord ).rgb);


	//resulting light
	vec3 result = ambient + diffuse + specular;
	//result += emission;

	FragColor =  vec4(result, 1.0);

}