#version 330 core

in vec3 ourColor; 
in vec4 vertexPos;

out vec4 FragColor; 

void main()
{
	FragColor = vec4(ourColor, 1.0); 
	//FragColor = vec4(vertexPos); 
}