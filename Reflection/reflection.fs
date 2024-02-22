#version 330 core 

out vec4 FragColor;

in vec3 Normal; 
in vec3 Position;

uniform vec3 cameraPos; 
uniform samplerCube skybox; 
uniform int choice;

void main()
{
	float ratio = 1.00 / 1.52;
	vec3 I = normalize(Position - cameraPos); 
	vec3 reflection = reflect(I, normalize(Normal)); 
	vec3 refraction = refract(I, normalize(Normal), ratio);

	switch(choice)
	{
		case 0: 
			FragColor = vec4(texture(skybox, reflection).rgb, 1.0);
			break;
		case 1: 
			FragColor = vec4(texture(skybox, refraction).rgb, 1.0);
			break;
		case 2:
			if(gl_FragCoord.x < 400) 
				FragColor = vec4(1.0, 0.0, 0.0, 1.0);
			else 
				FragColor = vec4(0.0, 1.0, 0.0, 1.0);
				break;

		default:
			FragColor = vec4(texture(skybox, reflection).rgb, 1.0);
			break;
	}
}