#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



using namespace std;

//Shader
//Vertex Shade
const char* vertexPath = "D:\\1. Programmieren\\OpenGL\\Programme\\7_Textures\\shader.vs";

//Fragment Shader
const char* fragmentPath = "D:\\1. Programmieren\\OpenGL\\Programme\\7_Textures\\shader.fs";


//--------------------------------------------------------------------------------------------

//Konstanten
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

//Variablen
float eingabe = 0.5f;
bool isPressed = false;

//Prototypen
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Shader ourShader);

//Vertices
float vertices[] = { 
	// positions      // colors         // texture coords
	0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right 
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right 
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
	-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
};

unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};

//OpenGL Objekte
unsigned int VBO;
unsigned int VAO;
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
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	//Setting VAOs
	//VAO
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//SHADER ----------------------------------------------------------------------------------------------------

	Shader ourShader(vertexPath, fragmentPath);


	//--------------------------------------------------------------------------------------------

	//Textures
	int width, height, nrChannels, width1, height1, nrChannels1;
	unsigned int texture1, texture2;

	glGenTextures(1, &texture1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	

	if (data)
	{
		//MipMap
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture1" << std::endl;
	}

	//free image memory
	stbi_image_free(data);


	glGenTextures(1, &texture2);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	unsigned char* data1 = stbi_load("awesomeface.png", &width1, &height1, &nrChannels1, 0);

	if (data1)
	{
		//MipMap
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture2" << std::endl;
	}

	//free image memory
	
	stbi_image_free(data1);



	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f); //Vektor, der transformiert werden soll
	glm::mat4 trans = glm::mat4(1.0f); //Einheitsmatrix

	




	//Render Loop
	while (!glfwWindowShouldClose(window))
	{
		//input
		processInput(window, ourShader);

		//rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Updates
		glm::mat4 trans = glm::mat4(1.0f);
		
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));		//Vektor gibt Rotationsachse an (NORMALISIERT!!!)
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		

		//uniform updates
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));

		//Draw some shit

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//Zweiter Container
		trans = glm::mat4(1.0f);
		trans = glm::scale(trans, glm::vec3(sin((float)glfwGetTime() * (float)glfwGetTime()), sin((float)glfwGetTime() * (float)glfwGetTime()), sin((float)glfwGetTime() * (float)glfwGetTime())));
		trans = glm::translate(trans, glm::vec3(sin((float)glfwGetTime() * -(float)glfwGetTime()), sin((float)glfwGetTime()), 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "transform"), 1, GL_FALSE, glm::value_ptr(trans));
		
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		ourShader.use();
		ourShader.setInt("texture1", 0);
		ourShader.setInt("texture2", 1);

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

void processInput(GLFWwindow* window, Shader ourShader)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && eingabe <= 0.9f && !isPressed)
	{
		cout << "pressed" << endl;
		eingabe += 0.1;
		ourShader.setFloat("eingabe", eingabe);
		isPressed = true;
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && eingabe >= 0.2 && !isPressed)
	{
		eingabe -= 0.1;
		ourShader.setFloat("eingabe", eingabe);
		isPressed = true;
	}
	else if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		isPressed = false;
	}

}