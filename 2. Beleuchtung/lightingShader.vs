#version 330 core

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aNormal; 
layout (location = 2) in vec2 aTexCoord; 

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;		//in Weltkoordinaten

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);		//in Screenkoordinaten
	TexCoord = aTexCoord;
	Normal = normalize(mat3(transpose(inverse(model))) * aNormal);
	FragPos = vec3(model * vec4(aPos, 1.0));					//die Modelmatrix transformiert aus Lokalkoordinaten in Weltkoordinaten
}