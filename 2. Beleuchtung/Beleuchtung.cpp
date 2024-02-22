#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include <Shader/Shader.h>
#include <stb_image/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Camera/camera.h>



using namespace std;

//Shader
//Vertex Shader
const char* cubeVertexPath = "D:\\1. Programmieren\\OpenGL\\Programme\\2. Beleuchtung\\lightingShader.vs";
const char* lightVertexPath = "D:\\1. Programmieren\\OpenGL\\Programme\\2. Beleuchtung\\lightSourceShader.vs";

//Fragment Shader
const char* cubeFragmentPath = "D:\\1. Programmieren\\OpenGL\\Programme\\2. Beleuchtung\\lightingShader.fs";
const char* lightFragmentPath = "D:\\1. Programmieren\\OpenGL\\Programme\\2. Beleuchtung\\lightSourceShader.fs";


//--------------------------------------------------------------------------------------------

//Konstanten
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;

//Texturen
int width, height, nrChannels;
unsigned int texture1, texture2;

//timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float currentFrame = 0.0f;
bool blinn = false;

//Kamera
Camera camera(glm::vec3(0.0f, 2.2f, 5.0f));
float lastMouseX = WINDOW_WIDTH/2.0f, lastMouseY = WINDOW_HEIGHT/2.0f;
bool firstMouse = true;

//Prototypen
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Shader lightingShader);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

//--------------------------------------------------------------------------------------------

//Vertices

float vertices[] = { 
	// positions    // normals // texture coords
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 
	-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),		
	glm::vec3(0.0f, 1.1f, 0.0f),
	glm::vec3(1.1f, 0.0f, 0.0f),
	glm::vec3(-1.1f, 0.0f, 0.0f),
	glm::vec3(-1.1, 1.1f, 0.0f),
	glm::vec3(1.1f, 1.1f, 0.0f),
	glm::vec3(1.1f, 2.2f, 0.0f),
	glm::vec3(-1.1f, 2.2f, 0.0f),
	glm::vec3(0.0f, 2.2f, 0.0f),
};

glm::vec3 ambients[]{
	glm::vec3(0.0215f, 0.1745f, 0.0215),
	glm::vec3(0.135f, 0.2225, 0.1575f),
	glm::vec3(0.05375f, 0.05f, 0.06625f),
	glm::vec3(0.25f, 0.20725f, 0.20725f),
	glm::vec3(0.1745,	0.01175	,0.01175),
	glm::vec3(0.1,	0.18725,	0.1745),
	glm::vec3(0.329412,	0.223529,	0.027451),
	glm::vec3(0.2125,	0.1275,	0.054),
	glm::vec3(0.25,	0.25,	0.25),
};

glm::vec3 diffuses[]{
	glm::vec3(0.07568f, 0.61424f, 0.07568f),
	glm::vec3(0.54f, 0.89f, 0.63f),
	glm::vec3(0.18275f, 0.17f, 0.22525f),
	glm::vec3(1.0f, 0.829f, 0.829f),
	glm::vec3(0.61424f, 0.04136f, 0.04136f),
	glm::vec3(0.396f, 0.74151f, 0.69102f),
	glm::vec3(0.780392f, 0.568627f, 0.113725f),
	glm::vec3(0.714f, 0.4284f, 0.18144f),
	glm::vec3(0.4f, 0.4f, 0.4f)
};

glm::vec3 speculars[]{
	glm::vec3(0.633f, 0.727811f, 0.633f),
	glm::vec3(0.316228f, 0.316228f, 0.316228f),
	glm::vec3(0.332741f, 0.328634f, 0.346435f),
	glm::vec3(0.296648f, 0.296648f, 0.296648f),
	glm::vec3(0.727811f, 0.6269459f, 0.626959),
	glm::vec3(0.297524f, 0.30829f, 0.306678f),
	glm::vec3(0.992157f, 0.941176f, 0.807843f),
	glm::vec3(0.393548f, 0.271906f, 0.166721f),
	glm::vec3(0.774597f, 0.774597f, 0.774597f),
};

float shininess[]{
	0.6f,
	0.2f,
	0.3f,
	0.088f,
	0.6f,
	0.1f,
	0.21794872f,
	0.2f,
	0.6f,
};

unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};

glm::vec3 cubePosition = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 lightPosition = glm::vec3(2.0f, 2.5f, -4.0f);

//--------------------------------------------------------------------------------------------

//OpenGL Objekte
unsigned int VBO;
unsigned int VAO;
unsigned int EBO;

unsigned int lightVAO;



//--------------------------------------------------------------------------------------------

//Kameradefinition
glm::mat4 view = glm::mat4(1.0f);


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
	glGenVertexArrays(1, &lightVAO);
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
	//normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	

	//lightVAO
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	//SHADER ----------------------------------------------------------------------------------------------------

	Shader lightingShader(cubeVertexPath, cubeFragmentPath);
	Shader lightSourceShader(lightVertexPath, lightFragmentPath);

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
	unsigned char* data = stbi_load("D:\\1. Programmieren\\OpenGL\\resources\\textures\\Container.jpg", &width, &height, &nrChannels, 0);


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
	data = stbi_load("D:\\1. Programmieren\\OpenGL\\resources\\textures\\awesomeface.png", &width, &height, &nrChannels, 0);

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
		float sinTime = sin((float)glfwGetTime());
		float cosTime = cos((float)glfwGetTime());

		glm::vec3 lightColor = glm::vec3(sinTime / 2 + 0.7, cosTime / 2 + 0.7, sinTime / 2 + 0.7);
		lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

		//rendering
		glClearColor((float)lightColor.r*0.5, (float)lightColor.g * 0.5, (float)lightColor.b * 0.5, 1.0f);

		//Bufferclear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//deltTime
		currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//input
		processInput(window, lightingShader);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);

		//Updates
		projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		view = camera.GetViewMatrix();


		float lichtBahnRadius = 3.0f;
		float lightX = sinTime * lichtBahnRadius;
		float lightZ = cosTime * lichtBahnRadius;
		lightPosition = glm::vec3(lightX, 1.0f, lightZ);
		lightPosition = glm::vec3(0.0f, 1.0f, 3.0f);

		glm::vec3 lightIntensity = glm::vec3((float)cos(glfwGetTime()));
		lightIntensity = glm::vec3(1.0f);

		//Cubes
		for (int i = 0; i < 9; i++)
		{
			lightingShader.use();
			
			//uniform updates
			lightingShader.setMat4("view", view);
			lightingShader.setMat4("projection", projection);
			lightingShader.setInt("texture1", 0);
			lightingShader.setInt("texture2", 1);

			lightingShader.setVec3("material.ambient", ambients[i]);
			lightingShader.setVec3("material.diffuse", diffuses[i]);
			lightingShader.setVec3("material.specular", speculars[i]);
			lightingShader.setFloat("material.shininess", shininess[i]*128);
			lightingShader.setBool("blinn", blinn);

			lightingShader.setVec3("light.position", lightPosition);
			lightingShader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
			lightingShader.setVec3("light.diffuse", lightColor * lightIntensity);
			lightingShader.setVec3("light.specular", lightColor);
		
			lightingShader.setVec3("viewPos", camera.Position);
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			lightingShader.setMat4("model", model);

			//Draw some shit
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, texture2);



			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}



		//Lichtquelle
		lightSourceShader.use();


		lightSourceShader.setVec3("lightColor", lightColor);
		lightSourceShader.setMat4("view", view);
		lightSourceShader.setMat4("projection", projection);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPosition);
		model = glm::scale(model, glm::vec3(0.2f));
		lightSourceShader.setMat4("model", model);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
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

void processInput(GLFWwindow* window, Shader lightingShader)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	//Kamerabewegung
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		blinn = true;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		blinn = false;
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

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}