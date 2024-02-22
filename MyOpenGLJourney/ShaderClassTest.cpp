#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include "Shader.h"


using namespace std;

//Shader
//Vertex Shade
const char* vertexPath = "D:\\1. Programmieren\\OpenGL\\Programme\\6. Shaders\\shader.vs";

//Fragment Shader
const char* fragmentPath = "D:\\1. Programmieren\\OpenGL\\Programme\\6. Shaders\\shader.fs";
const char* fragmentPath1 = "D:\\1. Programmieren\\OpenGL\\Programme\\6. Shaders\\shader1.fs";

//--------------------------------------------------------------------------------------------

//Konstanten
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

//Variablen
float timeValue = glfwGetTime();
float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

//Prototypen
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//Rechteckskoordinaten
float vertices[] = {
	-0.45f, 0.55f, 0.0f, //top left
	0.55f, 0.55f, 0.0f, //top right
	0.55f, -0.45f, 0.0f, //bottom right

	-0.55f, 0.45f, 0.0f,	//top left
	0.45f, -0.55f, 0.0f,	//bottom right
	-0.55f, -0.55f, 0.0f, //bottom left
};

float vertices0[] = {
	0.5f, 0.5f, 0.0f, // top right 
	0.5f, -0.5f, 0.0f, // bottom right 
	-0.5f, -0.5f, 0.0f, // bottom left 
	-0.5f, 0.5f, 0.0f // top left
};

float vertices1[] = {
	0.0f, 0.5f, 0.0f , 0.0f, 0.0f, 1.0f, //top 
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, //bottom left
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f//bottom right
};

float vertices2[] = {
	-0.55f, 0.45f, 0.0f, 0.0f, 0.0f, 1.0f, 	//top left
	0.45f, -0.55f, 0.0f, 0.0f, 1.0f, 0.0f, 	//bottom right
	-0.55f, -0.55f, 0.0f, 1.0f, 0.0f, 0.0f //bottom left
};


//indices für index drawing
unsigned int indices[] = {
	0, 1, 3,	//erstes Dreieck
	1, 2, 3		//zweites Dreieck
};

//OpenGL Objekte
unsigned int VBO;
unsigned int VBO1;
unsigned int VBO2;
unsigned int VAO;
unsigned int VAO1;
unsigned int VAO2;
unsigned int EBO;

//Shader


//ErrorHandling


//--------------------------------------------------------------------------------------------

int main()
{
	//Initialisiere GLFW und mache Version 3.3 und Core bekannt
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Erstelle window und setze den Kontext darauf
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "MyOpengGLJourney", NULL, NULL);

	if (window == NULL)
	{
		cout << "Failed to create GLFW windowww" << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//Lade GlAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLADDD" << endl;
		return -1;
	}

	//definiere OpenGL Viewport
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	//Rezising des Fensters soll auch den Viewport anpassen
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);






	//BUFFER ----------------------------------------------------------------------------------------------------
	//OpenGL Objektzuweisung
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);	
	glGenBuffers(1, &VBO1);
	glGenVertexArrays(1, &VAO1);	
	glGenBuffers(1, &VBO2);
	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &EBO);

	//Setting VAOs
	//VAO
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//VAO1
	glBindVertexArray(VAO1);

	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);

	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
	glEnableVertexAttribArray(1);

	//VAO2
	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);







	//SHADER ----------------------------------------------------------------------------------------------------

	Shader ourShader(vertexPath, fragmentPath);
	//Shader ourShader1(vertexPath, fragmentPath1);


	//--------------------------------------------------------------------------------------------
	
	//Render Loop
	while (!glfwWindowShouldClose(window))
	{
		//input
		processInput(window);

		//rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT);

		ourShader.use();

		//uniform updates
		float time = glfwGetTime();
		ourShader.setFloat("offset", sin(time)/2);
		ourShader.setFloat("r", sin(time + 1.0f) / 2 +0.5f);
		ourShader.setFloat("g", sin(time + 2.0f) / 2 + 0.5f);
		ourShader.setFloat("b", sin(time + 3.0f) / 2 + 0.5f);

		//Draw some shit
		
	
		

		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		//Bufferoperation und Events
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}

//--------------------------------------------------------------------------------------------

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		cout << "UP" << endl;
	}
		
}