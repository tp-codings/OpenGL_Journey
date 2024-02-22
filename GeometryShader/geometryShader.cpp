#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <learnopengl/filesystem.h>
#include <Shader/shader.h>
#include <Camera/camera.h>
#include <ModelLoader/model.h>

#include <iostream>


using namespace std;

//Basics--------------------------------------------------------------------------------------------
// 
//Window
int WINDOW_WIDTH = 800;
int WINDOW_HEIGHT = 600;
GLFWwindow* window;

//timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float currentFrame = 0.0f;

//Kamera
Camera camera(glm::vec3(0.0f, 2.2f, 5.0f));
float lastMouseX = WINDOW_WIDTH / 2.0f, lastMouseY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;

//Shader--------------------------------------------------------------------------------------------

Shader GS_Shader;
Shader screenShader;

Shader defaultShader;
Shader normalVisualizationShader;

Shader quadShader;

//Vertex Shader
const char* pointVertexPath = "D:\\1. Programmieren\\OpenGL\\Programme\\GeometryShader\\GS.vs";
const char* screenVertexPath = "D:\\1. Programmieren\\OpenGL\\Programme\\GeometryShader\\screen.vs";

const char* defaultVertexPath = "D:\\1. Programmieren\\OpenGL\\Programme\\GeometryShader\\default.vs";
const char* normalVertexPath = "D:\\1. Programmieren\\OpenGL\\Programme\\GeometryShader\\normal.vs";

const char* quadVertexPath = "D:\\1. Programmieren\\OpenGL\\Programme\\GeometryShader\\quad.vs";


//Fragment Shader
const char* pointFragmentPath = "D:\\1. Programmieren\\OpenGL\\Programme\\GeometryShader\\GS.fs";
const char* screenFragmentPath = "D:\\1. Programmieren\\OpenGL\\Programme\\GeometryShader\\screen.fs";

const char* defaultFragmentPath = "D:\\1. Programmieren\\OpenGL\\Programme\\GeometryShader\\default.fs";
const char* normalFragmentPath = "D:\\1. Programmieren\\OpenGL\\Programme\\GeometryShader\\normal.fs";

const char* quadFragmentPath = "D:\\1. Programmieren\\OpenGL\\Programme\\GeometryShader\\quad.fs";

//Geometry Shader
const char* pointGeometryPath = "D:\\1. Programmieren\\OpenGL\\Programme\\GeometryShader\\GS.gs";
const char* normalGeometryPath = "D:\\1. Programmieren\\OpenGL\\Programme\\GeometryShader\\normal.gs";

//Matrizen--------------------------------------------------------------------------------------------
glm::mat4 projection;
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 model = glm::mat4(1.0f);
glm::vec3 cubePosition = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cubePosition2 = glm::vec3(0.0f, 4.0f, 0.0f);

//Worldobjekte--------------------------------------------------------------------------------------------

float points[] = { 
	-0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // top-left 
	0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // top-right 
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right 
	-0.5f, -0.5f, 1.0f, 1.0f, 0.0f // bottom-left
};

float screenVertices[] = {
	// positions // texCoords
	-1.0f, 1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 1.0f, 0.0f,

	-1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f
};


float quadVertices[] = { 
	// positions // colors 
	-0.05f, 0.05f, 1.0f, 0.0f, 0.0f, 
	0.05f, -0.05f, 0.0f, 1.0f, 0.0f, 
	-0.05f, -0.05f, 0.0f, 0.0f, 1.0f,

	-0.05f, 0.05f, 1.0f, 0.0f, 0.0f, 
	0.05f, -0.05f, 0.0f, 1.0f, 0.0f, 
	0.05f, 0.05f, 0.0f, 1.0f, 1.0f
};

glm::vec2 translations[100];

//Textures und Lightmaps

//Buffer--------------------------------------------------------------------------------------------

unsigned int VAO;
unsigned int VBO;
unsigned int EBO;

unsigned int screenVAO;
unsigned int screenVBO;

unsigned int quadVAO;
unsigned int quadVBO;
unsigned int instanceVBO;

unsigned int framebuffer;
unsigned int texColorBuffer;
unsigned int rbo;

//Prototypen--------------------------------------------------------------------------------------------

int initWindow();
void initBuffer();
void initShader();
void initTextures();

void updateDeltaTime();
void updateCamera();

void DrawScene1();
void DrawScreen();

void input();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Shader GS_Shader);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(char const* path);
unsigned int loadCubeMap(vector<std::string> faces);

Model backpack;

int choice = 0;
//--------------------------------------------------------------------------------------------


int main()
{

	int index = 0;
	float offset = 0.1f;
	for (int y = -10; y < 10; y += 2)
	{
		for (int x = -10; x < 10; x += 2)
		{
			glm::vec2 translation;
			translation.x = (float)x / 10.0f + offset;
			translation.y = (float)y / 10.0f + offset;
			translations[index++] = translation;
		}
	}

	//WINDOW--------------------------------------------------------------------------------------------
	initWindow();

	//BUFFER ----------------------------------------------------------------------------------------------------
	initBuffer();


	//SHADER ----------------------------------------------------------------------------------------------------
	initShader();


	//Textures--------------------------------------------------------------------------------------------
	initTextures();

	//Models
	stbi_set_flip_vertically_on_load(true);
	backpack = Model("D:\\1. Programmieren\\OpenGL\\resources\\models\\backpack\\backpack.obj");

	//Tests--------------------------------------------------------------------------------------------
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_PROGRAM_POINT_SIZE);

	//glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);






	//Render Loop--------------------------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		//Model
		DrawScene1();
		
		DrawScreen();

		//Bufferoperation und Events
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}


void DrawScene1()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//Bufferclear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//input
	input();

	//deltTime
	updateDeltaTime();

	//Updates
	projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);

	switch (choice)
	{
	case 0:
		GS_Shader.use();
		GS_Shader.setMat4("projection", projection);
		GS_Shader.setMat4("view", view);
		GS_Shader.setMat4("model", model);
		GS_Shader.setFloat("time", static_cast<float>(glfwGetTime()));

		backpack.Draw(GS_Shader);
		break;
	case 1:
		defaultShader.use();
		defaultShader.setMat4("projection", projection);
		defaultShader.setMat4("view", view);
		defaultShader.setMat4("model", model);
		backpack.Draw(defaultShader);

		normalVisualizationShader.use();
		normalVisualizationShader.setMat4("projection", projection);
		normalVisualizationShader.setMat4("view", view);
		normalVisualizationShader.setMat4("model", model);
		backpack.Draw(normalVisualizationShader);
		break;
	case 2:
		quadShader.use();
		glBindVertexArray(quadVAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
		break;
	default:
		defaultShader.use();
		defaultShader.setMat4("projection", projection);
		defaultShader.setMat4("view", view);
		defaultShader.setMat4("model", model);
		backpack.Draw(defaultShader);
		break;
	}

	/*GS_Shader.use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, 4);*/
}


void DrawScreen()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	screenShader.use();
	glBindVertexArray(screenVAO);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}





//Inits--------------------------------------------------------------------------------------------

int initWindow()
{
	//Initialisiere GLFW und mache Version 3.3 und Core bekannt
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Erstelle window und setze den Kontext darauf
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "MyOpengGLJourney", NULL, NULL);

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
}

void initBuffer()
{




	//Haus
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2* sizeof(float)));
	glEnableVertexAttribArray(1);

	//Quads
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//Instancearray
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, &translations[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(2, 1); // -> Instanced Array -> nur bei jeder neuen Instanz aufrufen

	//Screen
	glGenVertexArrays(1, &screenVAO);
	glGenBuffers(1, &screenVBO);
	glBindVertexArray(screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
	//position attribute
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//texture attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//Screen Framebuffer
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// create a color attachment texture
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void initShader()
{
	GS_Shader = Shader(pointVertexPath, pointFragmentPath, pointGeometryPath);

	screenShader = Shader(screenVertexPath, screenFragmentPath);

	defaultShader = Shader(defaultVertexPath, defaultFragmentPath);

	normalVisualizationShader = Shader(normalVertexPath, normalFragmentPath, normalGeometryPath);

	quadShader = Shader(quadVertexPath, quadFragmentPath);
}

void initTextures()
{

}

void updateDeltaTime()
{
	currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}



//Handler und Zeugs--------------------------------------------------------------------------------------------


void input()
{
	processInput(window, GS_Shader);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, Shader GS_Shader)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
	{
		choice = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		choice = 1;
	}	
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		choice = 2;
	}

	//Kamerabewegung
	updateCamera();
}

void updateCamera() {
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);
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

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if (format = GL_RGBA)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

unsigned int loadCubeMap(vector<std::string> faces)
{
	unsigned int textureID;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);

		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

