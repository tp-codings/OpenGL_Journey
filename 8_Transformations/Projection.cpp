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
const char* vertexPath = "D:\\1. Programmieren\\OpenGL\\Programme\\8_Transformations\\vertexShader.vs";

//Fragment Shader
const char* fragmentPath = "D:\\1. Programmieren\\OpenGL\\Programme\\8_Transformations\\fragmentShader.fs";


//--------------------------------------------------------------------------------------------

//Konstanten
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

//Variablen
int width, height, nrChannels;
unsigned int texture1, texture2;

float eingabe = 0.5f;
bool isPressed = false;

const float kameraBahnRadius = 40.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float currentFrame = 0.0f;

float yaw = -90.0f;
float pitch = 0.0f;
float lastMouseX = 400, lastMouseY = 300;
bool firstMouse = true;
float fov = 45.0f;
float zoom = 45.0f;

//Prototypen
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Shader ourShader);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//--------------------------------------------------------------------------------------------

//Vertices
float vertices0[] = {
	// positions      // colors         // texture coords
	0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right 
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right 
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
	-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
};

float vertices[] = { 
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};


unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};

glm::vec3 cubePositions[] = { 
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, 5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f, 3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f, 2.0f, -2.5f),
	glm::vec3(1.5f, 0.2f, -1.5f),
	glm::vec3(-1.3f, 1.0f, -1.5f)
};

//--------------------------------------------------------------------------------------------

//OpenGL Objekte
unsigned int VBO;
unsigned int VAO;
unsigned int EBO;

//--------------------------------------------------------------------------------------------

//Kameradefinition nach Gram-Schmidt-Prozess
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
glm::vec3 cameraRight = glm::normalize(glm::cross(cameraUp, cameraDirection));
glm::vec3 up = glm::cross(cameraDirection, cameraRight);

glm::mat4 view = glm::mat4(1.0f);

glm::vec4 direction;

//--------------------------------------------------------------------------------------------

//Matrizen zur Transformation von Objekten
glm::mat4 model = glm::mat4(1.0f);
glm::mat4 projection;

//--------------------------------------------------------------------------------------------


int main()
{
	//WINDOW--------------------------------------------------------------------------------------------
	
	//Initialisiere GLFW und mache Version 3.3 und Core bekannt
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Erstelle window und setze den Kontext darauf
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "MyOpengGLJourney", NULL, NULL);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color attribute
	/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);*/
	//texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//SHADER ----------------------------------------------------------------------------------------------------

	Shader ourShader(vertexPath, fragmentPath);

	//Textures--------------------------------------------------------------------------------------------
	
	//Containerbild
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

	//Smileybild
	glGenTextures(1, &texture2);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);

	if (data)
	{
		//MipMap
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture2" << std::endl;
	}

	//free image memory

	stbi_image_free(data);

	//--------------------------------------------------------------------------------------------


	//ZUWEISUNGEN--------------------------------------------------------------------------------------------



	//Tiefentest, benötigt aber auch das Löschen des Depth-Buffers
	glEnable(GL_DEPTH_TEST);
	

	


	//Render Loop--------------------------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{

		//rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		//Bufferclear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//deltTime
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//input
		processInput(window, ourShader);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);

		//Updates
		float camX = sin(glfwGetTime()) * kameraBahnRadius;
		float camZ = cos(glfwGetTime()) * kameraBahnRadius;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);

		//Shaderuse
		ourShader.use();

		//uniform updates
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		ourShader.setInt("texture1", 0);
		ourShader.setInt("texture2", 1);

		//Draw some shit
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glBindVertexArray(VAO);

		for (int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f) * (i+1) , glm::vec3(0.5f , 1.0f , 0.0f));
			glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(0);


		//Bufferoperation und Events
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}

//Funktionen--------------------------------------------------------------------------------------------

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
	else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		isPressed = false;
	}
	//Kamerabewegung
	const float cameraSpeed = 4.5f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraPos += cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraPos -= cameraSpeed * cameraFront;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraPos -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraPos += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
	}

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastMouseX = xpos;
		lastMouseY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastMouseX;
	float yoffset = lastMouseY - ypos;

	lastMouseX = xpos;
	lastMouseY = ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	direction.x = cos(glm::radians(yaw) * cos(glm::radians(pitch)));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw) * cos(glm::radians(pitch)));

	cameraFront = glm::normalize(direction);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	zoom -= (float)yoffset;
	if (zoom < 1.0f)
		zoom = 1.0f;
	if (zoom > 45.0f)
		zoom = 45.0f;
	fov = zoom;  
}