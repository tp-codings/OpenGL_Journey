#version 330 core

layout (location = 0) in vec3 aPos; 
layout (location = 1) in vec3 aColor; 

out vec3 ourColor; 
out vec4 vertexPos;

uniform float offset;
uniform float r;
uniform float g;
uniform float b;

void main()
{
	gl_Position = vec4(aPos.x+offset, aPos.y, aPos.z, 1.0); 
	vertexPos = vec4(aPos.x, aPos.y, aPos.z, 1.0); 
	ourColor = vec3(aColor.r*r, aColor.g*g, aColor.b*b); ; 
}