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


#define ANZ_AMBIENTE 4
#define ANZ_POINTLIGHTS 4

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

Shader lightingShader;
Shader lightSourceShader;
Shader borderShader;
Shader transparencyShader;
Shader screenShader;
Shader rearScreenShader;
Shader cubemapShader;

//Vertex Shader
const char* cubeVertexPath = "D:\\1. Programmieren\\OpenGL\\Programme\\Postprocessing\\lightingShader.vs";
const char* lightVertexPath = "D:\\1. Programmieren\\OpenGL\\Programme\\Postprocessing\\lightSourceShader.vs";
const char* screenVertexPath = "D:\\1. Programmieren\\OpenGL\\Programme\\Postprocessing\\screen.vs";
const char* cubemapVertexPath = "D:\\1. Programmieren\\OpenGL\\Programme\\Postprocessing\\cubemap.vs";

//Fragment Shader
const char* cubeFragmentPath = "D:\\1. Programmieren\\OpenGL\\Programme\\Postprocessing\\lightingShader.fs";
const char* lightFragmentPath = "D:\\1. Programmieren\\OpenGL\\Programme\\Postprocessing\\lightSourceShader.fs";
const char* borderFragmentPath = "D:\\1. Programmieren\\OpenGL\\Programme\\Postprocessing\\Border.fs";
const char* transparencyFragmentPath = "D:\\1. Programmieren\\OpenGL\\Programme\\Postprocessing\\transparency.fs";
const char* screenFragmentPath = "D:\\1. Programmieren\\OpenGL\\Programme\\Postprocessing\\screen.fs";
const char* rearScreenFragmentPath = "D:\\1. Programmieren\\OpenGL\\Programme\\Postprocessing\\rearScreen.fs";
const char* cubemapFragmentPath = "D:\\1. Programmieren\\OpenGL\\Programme\\Postprocessing\\cubemap.fs";

//Ambiente 
enum Ambiente {DESERT, FACTORY, HORROR, BIOLAB};
enum Ambiente atmosphere = DESERT;

//pointlight choice
int pointlightChoice = 0;
float oldQuadratic = 0.032f;
glm::vec3 oldColor = glm::vec3(1, 0.89, 0.39);
bool lightOffPressed = false;


bool spotlight = true;
float cutOff = 12.5f;
float outerCutOff = 17.5f;
float oldCutOff;
float oldOuterCutOff;
bool spotlightOn = true;
bool spotlightOff = false;

glm::vec3 lightIntensity;

//glm::vec3 lightColor[] = 
//{
//	glm::vec3(1, 0.89, 0.39), //DESERT
//	glm::vec3(0.22f, 0.35f, 0.53f),	//FACTORY
//	glm::vec3(0.84f, 0.0f, 0.0f),	//HORROR
//	glm::vec3(0.35f, 0.67f, 0.38f),	//BIOLAB
//};
glm::vec4 backGroundColor[] = 
{
	glm::vec4(0.75f, 0.52f, 0.3f, 1.0f), //DESERT
	glm::vec4(0.1f, 0.1f, 0.1f, 1.0f),	//FACTORY
	glm::vec4(0.0f, 0.0f, 0.0f, 1.0f),	//HORROR
	glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)	//BIOLAB
};

struct DirectionalLight
{
	glm::vec3 direction[4];
	glm::vec3 ambient[4];
	glm::vec3 diffuse[4];
	glm::vec3 specular[4];
} dirLight;

struct PointLight
{
	glm::vec3 lightColor[4];
	glm::vec3 ambient[4];
	glm::vec3 diffuse[4];
	glm::vec3 specular[4];
	float constant[4];
	float linear[4];
	float quadratic[4];
} pointLight[4];




//Matrizen--------------------------------------------------------------------------------------------
glm::mat4 projection;
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 model = glm::mat4(1.0f);

glm::vec3 cubePosition = glm::vec3(0.0f, 0.0f, 0.0f);

//Worldobjekte--------------------------------------------------------------------------------------------
//Cube
float vertices[] =
{
	// positions          // normals           // texture coords
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

float cubemapVertices[] =
{
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
};

glm::vec3 cubePositions[] =
{
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.1f, 0.0f),
	glm::vec3(1.1f, 0.0f, 0.0f),
	glm::vec3(-1.1f, 0.0f, 0.0f),
	glm::vec3(-1.1, 1.1f, 0.0f),
	glm::vec3(1.1f, 1.1f, 0.0f),
	glm::vec3(1.1f, 2.2f, 0.0f),
	glm::vec3(-1.1f, 2.2f, 0.0f),
	glm::vec3(0.0f, 2.2f, 0.0f),
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

glm::vec3 pointLightPositions[] = 
{ 
	glm::vec3(0.7f, 0.2f, 2.0f), 
	glm::vec3(2.3f, -3.3f, -4.0f), 
	glm::vec3(-4.0f, 2.0f, -12.0f), 
	glm::vec3(0.0f, 0.0f, -3.0f)
};

unsigned int indices[] = 
{
	0, 1, 3,
	1, 2, 3
};


vector<glm::vec3> vegetation; 
float vegetationQuad[] =
{
	// positions          // normals           // texture coords
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
};

float quadVertices[] = { // positions // texCoords
	-1.0f, 1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 1.0f, 0.0f,

	-1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f
};

float rawQuadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates. NOTE that this plane is now much smaller and at the top of the screen
	 // positions   // texCoords
	 -0.3f,  1.0f,  0.0f, 1.0f,
	 -0.3f,  0.7f,  0.0f, 0.0f,
	  0.3f,  0.7f,  1.0f, 0.0f,

	 -0.3f,  1.0f,  0.0f, 1.0f,
	  0.3f,  0.7f,  1.0f, 0.0f,
	  0.3f,  1.0f,  1.0f, 1.0f
};

//Textures und Lightmaps
unsigned int diffuseMap;
unsigned int specularMap;
unsigned int emissionMap;

unsigned int grassTex;

unsigned int cubemapTexture;

vector<std::string> faces
{
	"D:\\1. Programmieren\\OpenGL\\resources\\textures\\skybox\\right.jpg",
	"D:\\1. Programmieren\\OpenGL\\resources\\textures\\skybox\\left.jpg",
	"D:\\1. Programmieren\\OpenGL\\resources\\textures\\skybox\\top.jpg",
	"D:\\1. Programmieren\\OpenGL\\resources\\textures\\skybox\\bottom.jpg",
	"D:\\1. Programmieren\\OpenGL\\resources\\textures\\skybox\\front.jpg",
	"D:\\1. Programmieren\\OpenGL\\resources\\textures\\skybox\\back.jpg"
};

//Buffer--------------------------------------------------------------------------------------------

unsigned int VAO;
unsigned int VBO;
unsigned int EBO;

unsigned int lightVAO;

unsigned int vegetationVAO;
unsigned int vegetationVBO;

unsigned int screenVAO;
unsigned int screenVBO;

unsigned int rearViewVAO;
unsigned int rearViewVBO;

unsigned int framebuffer;
unsigned int texColorBuffer;
unsigned int rbo;

unsigned int RVFramebuffer;
unsigned int RVTexColorBuffer;
unsigned int rearViewRBO;

unsigned int cubemapVAO;
unsigned int cubemapVBO;

//Prototypen--------------------------------------------------------------------------------------------

int initWindow();
void initBuffer();
void initShader();
void initTextures();
void initLightning();

void updateDeltaTime();
void updateCamera();
void updateSpotlight();
void updateAmbiente();
void updateLightPosition();

void DrawScene1();
void DrawScene2();
void DrawScreen();

void input();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Shader lightingShader);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(char const* path);
unsigned int loadCubeMap(vector<std::string> faces);


//--------------------------------------------------------------------------------------------


int main()
{
	//WINDOW--------------------------------------------------------------------------------------------
	initWindow();
	
	//BUFFER ----------------------------------------------------------------------------------------------------
	initBuffer();


	//SHADER ----------------------------------------------------------------------------------------------------
	initShader();


	//Textures--------------------------------------------------------------------------------------------
	initTextures();

	//Beleuchtung
	initLightning();

	//Models
	stbi_set_flip_vertically_on_load(true);
	//Model backpack("D:\\1. Programmieren\\OpenGL\\resources\\models\\backpack\\backpack.obj");

	//Tests--------------------------------------------------------------------------------------------
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Render Loop--------------------------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{



		DrawScene2();
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
	glClearColor(backGroundColor[atmosphere].r, backGroundColor[atmosphere].g, backGroundColor[atmosphere].b, backGroundColor[atmosphere].a);

	//Bufferclear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//input
	input();

	//deltTime
	updateDeltaTime();

	//Skybox
	glDepthMask(GL_FALSE);
	cubemapShader.use();

	projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
	view = glm::mat4(glm::mat3(camera.GetViewMatrix()));

	cubemapShader.setMat4("view", view);
	cubemapShader.setMat4("projection", projection);
	cubemapShader.setInt("skybox", 0);

	glBindVertexArray(cubemapVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
	glDepthMask(GL_TRUE);

	//Updates
	projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
	view = camera.GetViewMatrix();

	float sinTime = sin((float)glfwGetTime());
	float cosTime = cos((float)glfwGetTime());

	//rendering Background

	//lightSourceOrbit
	float lichtBahnRadius = 3.0f;
	float lightX = sinTime * lichtBahnRadius;
	float lightZ = cosTime * lichtBahnRadius;
	glm::vec3 lightPosition = glm::vec3(lightX, 1.0f, lightZ);
	//lightPosition = glm::vec3(0.0f, 1.0f, 3.0f);

	lightIntensity = glm::vec3((float)cos(glfwGetTime()));
	lightIntensity = glm::vec3(0.5f);

	//Settings
	glStencilMask(0x00);

	transparencyShader.use();

	transparencyShader.setMat4("view", view);
	transparencyShader.setMat4("projection", projection);
	transparencyShader.setInt("texture1", 0);


	lightingShader.use();

	lightingShader.setMat4("view", view);
	lightingShader.setMat4("projection", projection);


	//Materialeigenschaften
	lightingShader.setFloat("material.shininess", 64.0f);
	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 1);
	lightingShader.setInt("material.emission", 2);

	//Lichteigenschaften
	//Directional light
	lightingShader.setVec3("dirLight.direction", dirLight.direction[atmosphere]);
	lightingShader.setVec3("dirLight.ambient", dirLight.ambient[atmosphere]);
	lightingShader.setVec3("dirLight.diffuse", dirLight.diffuse[atmosphere]);
	lightingShader.setVec3("dirLight.specular", dirLight.specular[atmosphere]);

	//Pointlights
	for (int i = 0; i < ANZ_POINTLIGHTS; i++) {
		lightingShader.setVec3("pointLights[" + to_string(i) + "].position", pointLightPositions[i]);
		lightingShader.setVec3("pointLights[" + to_string(i) + "].ambient", pointLight[i].ambient[atmosphere]);
		lightingShader.setVec3("pointLights[" + to_string(i) + "].diffuse", pointLight[i].diffuse[atmosphere]);
		lightingShader.setVec3("pointLights[" + to_string(i) + "].specular", pointLight[i].specular[atmosphere]);
		lightingShader.setFloat("pointLights[" + to_string(i) + "].constant", pointLight[i].constant[atmosphere]);
		lightingShader.setFloat("pointLights[" + to_string(i) + "].linear", pointLight[i].linear[atmosphere]);
		lightingShader.setFloat("pointLights[" + to_string(i) + "].quadratic", pointLight[i].quadratic[atmosphere]);
	}

	//spot light
	lightingShader.setVec3("spotLight.ambient", 0.00f, 0.00f, 0.00f);
	lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	lightingShader.setFloat("spotLight.constant", 1.0f);
	lightingShader.setFloat("spotLight.linear", 0.09f);
	lightingShader.setFloat("spotLight.quadratic", 0.032f);

	lightingShader.setVec3("viewPos", camera.Position);
	lightingShader.setFloat("time", (float)glfwGetTime());

	lightingShader.setVec3("spotLight.sourceDirection", camera.Front);
	lightingShader.setVec3("spotLight.position", camera.Position);
	lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(cutOff)));
	lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(outerCutOff)));

	model = glm::mat4(1.0f);
	lightingShader.setMat4("model", model);





	//Model
	//backpack.Draw(lightingShader);

	//Cubeupdates
	for (int i = 11; i < 14; i++)
	{
		lightingShader.use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f) * (1 + i) * (float)0.05, glm::vec3(0.5f, 1.0f, 0.0f) * glm::vec3(sin(i), -sin(i), sin(i)));
		lightingShader.setMat4("model", model);


		if (i != 12)
		{
			//Container 2 (diffuse Map)
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuseMap);

			//Container 2 (specular Map)
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, specularMap);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, emissionMap);

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		else
		{
			transparencyShader.use();
			transparencyShader.setMat4("model", model);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, grassTex);

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}


	//Lichtquelle
	lightSourceShader.use();

	lightSourceShader.setMat4("view", view);
	lightSourceShader.setMat4("projection", projection);

	borderShader.use();
	borderShader.setMat4("view", view);
	borderShader.setMat4("projection", projection);

	for (int i = 0; i < 4; i++)
	{
		lightSourceShader.use();
		if (atmosphere == HORROR)
		{
			lightSourceShader.setVec3("lightColor", pointLight[i].lightColor[atmosphere] * lightIntensity);
		}
		else
		{
			lightSourceShader.setVec3("lightColor", pointLight[i].lightColor[atmosphere]);
		}

		model = glm::mat4(1.0f);
		model = glm::translate(model, pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(0.2f));
		lightSourceShader.setMat4("model", model);

		//First

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		if (pointlightChoice == i)
		{
			//Second
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			//glDisable(GL_DEPTH_TEST);
			borderShader.use();
			glBindVertexArray(lightVAO);
			model = glm::scale(model, glm::vec3(1.2f));
			borderShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, 0, 0xFF);
			glEnable(GL_DEPTH_TEST);
		}


	}

	//Transparent

	std::map<float, glm::vec3> sorted;
	for (unsigned int i = 0; i < vegetation.size(); i++)
	{
		float distance = glm::length(camera.Position - vegetation[i]);
		sorted[distance] = vegetation[i];
	}

	transparencyShader.use();
	glBindVertexArray(vegetationVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grassTex);

	for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
	{
		model = glm::mat4(1.0f);
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 
		model = glm::translate(model, it->second);
		transparencyShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	//for (unsigned int i = 0; i < vegetation.size(); i++) 
	//{
	//	model = glm::mat4(1.0f); 
	//	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 
	//	model = glm::translate(model, vegetation[i]); 
	//	transparencyShader.setMat4("model", model); 
	//	glDrawArrays(GL_TRIANGLES, 0, 6);
	//}

	glBindVertexArray(0);


}

void DrawScene2()
{
	glBindFramebuffer(GL_FRAMEBUFFER, RVFramebuffer);
	glEnable(GL_DEPTH_TEST);
	glClearColor(backGroundColor[atmosphere].r, backGroundColor[atmosphere].g, backGroundColor[atmosphere].b, backGroundColor[atmosphere].a);

	//Bufferclear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//input
	input();

	//deltTime
	updateDeltaTime();



	//Skybox
	glDepthMask(GL_FALSE);
	cubemapShader.use();

	projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
	camera.Yaw += 180.0f; // rotate the camera's yaw 180 degrees around
	camera.ProcessMouseMovement(0, 0, false); // call this to make sure it updates its camera vectors, note that we disable pitch constrains for this specific case (otherwise we can't reverse camera's pitch values)
	view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
	camera.Yaw -= 180.0f; // reset it back to its original orientation
	camera.ProcessMouseMovement(0, 0, true);

	cubemapShader.setMat4("view", view);
	cubemapShader.setMat4("projection", projection);
	cubemapShader.setInt("skybox", 0);

	glBindVertexArray(cubemapVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
	glDepthMask(GL_TRUE);




	//Updates
	projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
	camera.Yaw += 180.0f; // rotate the camera's yaw 180 degrees around
	camera.ProcessMouseMovement(0, 0, false); // call this to make sure it updates its camera vectors, note that we disable pitch constrains for this specific case (otherwise we can't reverse camera's pitch values)
	view = camera.GetViewMatrix();
	camera.Yaw -= 180.0f; // reset it back to its original orientation
	camera.ProcessMouseMovement(0, 0, true);

	float sinTime = sin((float)glfwGetTime());
	float cosTime = cos((float)glfwGetTime());

	//rendering Background

	//lightSourceOrbit
	float lichtBahnRadius = 3.0f;
	float lightX = sinTime * lichtBahnRadius;
	float lightZ = cosTime * lichtBahnRadius;
	glm::vec3 lightPosition = glm::vec3(lightX, 1.0f, lightZ);
	//lightPosition = glm::vec3(0.0f, 1.0f, 3.0f);

	lightIntensity = glm::vec3((float)cos(glfwGetTime()));
	lightIntensity = glm::vec3(0.5f);

	//Settings
	glStencilMask(0x00);

	transparencyShader.use();

	transparencyShader.setMat4("view", view);
	transparencyShader.setMat4("projection", projection);
	transparencyShader.setInt("texture1", 0);


	lightingShader.use();

	lightingShader.setMat4("view", view);
	lightingShader.setMat4("projection", projection);


	//Materialeigenschaften
	lightingShader.setFloat("material.shininess", 64.0f);
	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 1);
	lightingShader.setInt("material.emission", 2);

	//Lichteigenschaften
	//Directional light
	lightingShader.setVec3("dirLight.direction", dirLight.direction[atmosphere]);
	lightingShader.setVec3("dirLight.ambient", dirLight.ambient[atmosphere]);
	lightingShader.setVec3("dirLight.diffuse", dirLight.diffuse[atmosphere]);
	lightingShader.setVec3("dirLight.specular", dirLight.specular[atmosphere]);

	//Pointlights
	for (int i = 0; i < ANZ_POINTLIGHTS; i++) {
		lightingShader.setVec3("pointLights[" + to_string(i) + "].position", pointLightPositions[i]);
		lightingShader.setVec3("pointLights[" + to_string(i) + "].ambient", pointLight[i].ambient[atmosphere]);
		lightingShader.setVec3("pointLights[" + to_string(i) + "].diffuse", pointLight[i].diffuse[atmosphere]);
		lightingShader.setVec3("pointLights[" + to_string(i) + "].specular", pointLight[i].specular[atmosphere]);
		lightingShader.setFloat("pointLights[" + to_string(i) + "].constant", pointLight[i].constant[atmosphere]);
		lightingShader.setFloat("pointLights[" + to_string(i) + "].linear", pointLight[i].linear[atmosphere]);
		lightingShader.setFloat("pointLights[" + to_string(i) + "].quadratic", pointLight[i].quadratic[atmosphere]);
	}

	//spot light
	lightingShader.setVec3("spotLight.ambient", 0.00f, 0.00f, 0.00f);
	lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	lightingShader.setFloat("spotLight.constant", 1.0f);
	lightingShader.setFloat("spotLight.linear", 0.09f);
	lightingShader.setFloat("spotLight.quadratic", 0.032f);

	lightingShader.setVec3("viewPos", camera.Position);
	lightingShader.setFloat("time", (float)glfwGetTime());

	lightingShader.setVec3("spotLight.sourceDirection", camera.Front);
	lightingShader.setVec3("spotLight.position", camera.Position);
	lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(cutOff)));
	lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(outerCutOff)));

	model = glm::mat4(1.0f);
	lightingShader.setMat4("model", model);



	//Model
	//backpack.Draw(lightingShader);

	//Cubeupdates
	for (int i = 11; i < 14; i++)
	{
		lightingShader.use();
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f) * (1 + i) * (float)0.05, glm::vec3(0.5f, 1.0f, 0.0f) * glm::vec3(sin(i), -sin(i), sin(i)));
		lightingShader.setMat4("model", model);


		if (i != 12)
		{
			//Container 2 (diffuse Map)
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, diffuseMap);

			//Container 2 (specular Map)
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, specularMap);

			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, emissionMap);

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		else
		{
			transparencyShader.use();
			transparencyShader.setMat4("model", model);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, grassTex);

			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}


	//Lichtquelle
	lightSourceShader.use();

	lightSourceShader.setMat4("view", view);
	lightSourceShader.setMat4("projection", projection);

	borderShader.use();
	borderShader.setMat4("view", view);
	borderShader.setMat4("projection", projection);

	for (int i = 0; i < 4; i++)
	{
		lightSourceShader.use();
		if (atmosphere == HORROR)
		{
			lightSourceShader.setVec3("lightColor", pointLight[i].lightColor[atmosphere] * lightIntensity);
		}
		else
		{
			lightSourceShader.setVec3("lightColor", pointLight[i].lightColor[atmosphere]);
		}

		model = glm::mat4(1.0f);
		model = glm::translate(model, pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(0.2f));
		lightSourceShader.setMat4("model", model);





		//First

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		if (pointlightChoice == i)
		{
			//Second
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			//glDisable(GL_DEPTH_TEST);
			borderShader.use();
			glBindVertexArray(lightVAO);
			model = glm::scale(model, glm::vec3(1.2f));
			borderShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, 0, 0xFF);
			glEnable(GL_DEPTH_TEST);
		}

		glBindVertexArray(0);



	}

	//Transparent

	std::map<float, glm::vec3> sorted;
	for (unsigned int i = 0; i < vegetation.size(); i++)
	{
		float distance = glm::length(camera.Position - vegetation[i]);
		sorted[distance] = vegetation[i];
	}

	transparencyShader.use();
	glBindVertexArray(vegetationVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grassTex);

	for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
	{
		model = glm::mat4(1.0f);
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 
		model = glm::translate(model, it->second);
		transparencyShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	//for (unsigned int i = 0; i < vegetation.size(); i++) 
	//{
	//	model = glm::mat4(1.0f); 
	//	model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)); 
	//	model = glm::translate(model, vegetation[i]); 
	//	transparencyShader.setMat4("model", model); 
	//	glDrawArrays(GL_TRIANGLES, 0, 6);
	//}
}

void DrawScreen()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	screenShader.use();
	glBindVertexArray(screenVAO);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	rearScreenShader.use();
	glBindVertexArray(rearViewVAO);
	glBindTexture(GL_TEXTURE_2D, RVTexColorBuffer);
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
	//CUbe
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
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


	//LightCubes
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	

	//Vegetation
	glGenVertexArrays(1, &vegetationVAO);
	glGenBuffers(1, &vegetationVBO);
	glBindVertexArray(vegetationVAO);
	glBindBuffer(GL_ARRAY_BUFFER, vegetationVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vegetationQuad), vegetationQuad, GL_STATIC_DRAW);
	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//texture attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	//Skybox
	glGenVertexArrays(1, &cubemapVAO);
	glGenBuffers(1, &cubemapVBO);
	glBindVertexArray(cubemapVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubemapVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubemapVertices), cubemapVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Screen
	glGenVertexArrays(1, &screenVAO);
	glGenBuffers(1, &screenVBO);
	glBindVertexArray(screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
	//position attribute
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
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
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600 ); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	//Rear view mirror
	glGenVertexArrays(1, &rearViewVAO);
	glGenBuffers(1, &rearViewVBO);
	glBindVertexArray(rearViewVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rearViewVBO);
	//position attribute
	glBufferData(GL_ARRAY_BUFFER, sizeof(rawQuadVertices), rawQuadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//texture attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//Rear View Mirror Framebuffer
	glGenFramebuffers(1, &RVFramebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, RVFramebuffer);
	// create a color attachment texture
	glGenTextures(1, &RVTexColorBuffer);
	glBindTexture(GL_TEXTURE_2D, RVTexColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, RVTexColorBuffer, 0);
	// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	glGenRenderbuffers(1, &rearViewRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, rearViewRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600); // use a single renderbuffer object for both a depth AND stencil buffer.
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rearViewRBO); // now actually attach it
	// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: RVFramebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void initShader()
{
	lightingShader = Shader(cubeVertexPath, cubeFragmentPath);
	transparencyShader = Shader(cubeVertexPath, transparencyFragmentPath);

	lightSourceShader = Shader(lightVertexPath, lightFragmentPath);
	borderShader = Shader(lightVertexPath, borderFragmentPath);

	screenShader = Shader(screenVertexPath, screenFragmentPath);
	rearScreenShader = Shader(screenVertexPath, rearScreenFragmentPath);

	cubemapShader = Shader(cubemapVertexPath, cubemapFragmentPath);
}

void initTextures()
{
	//Containerbild
	diffuseMap = loadTexture("D:\\1. Programmieren\\OpenGL\\resources\\textures\\container2.png");
	specularMap = loadTexture("D:\\1. Programmieren\\OpenGL\\resources\\textures\\container2_specular.png");
	emissionMap = loadTexture("D:\\1. Programmieren\\OpenGL\\resources\\textures\\matrix.jpg");

	//grassTex = loadTexture("D:\\1. Programmieren\\OpenGL\\resources\\textures\\blending_transparent_window.png");
	grassTex = loadTexture("D:\\1. Programmieren\\OpenGL\\resources\\textures\\dart2.jpeg");

	cubemapTexture = loadCubeMap(faces);

	vegetation.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	vegetation.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
	vegetation.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	vegetation.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	vegetation.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

}

void updateDeltaTime()
{
	currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void initLightning()
{
	//Directional Light
	//DESERT
	dirLight.direction[0] = glm::vec3(-0.2f, -1.0f, -0.3f); 
	dirLight.ambient[0] = glm::vec3(0.3f, 0.24f, 0.14f);
	dirLight.diffuse[0] = glm::vec3(0.7f, 0.42f, 0.26f);
	dirLight.specular[0] = glm::vec3(0.5f, 0.5f, 0.5f);


	//FACTORY
	dirLight.direction[1] = glm::vec3(-0.2f, -1.0f, -0.3f);
	dirLight.ambient[1] = glm::vec3(0.05f, 0.05f, 0.1f);
	dirLight.diffuse[1] = glm::vec3(0.2f, 0.2f, 0.7f);
	dirLight.specular[1] = glm::vec3(0.7f, 0.7f, 0.7f);

	//HORROR
	dirLight.direction[2] = glm::vec3(-0.2f, -1.0f, -0.3f);
	dirLight.ambient[2] = glm::vec3(0.0f, 0.0f, 0.0f);
	dirLight.diffuse[2] = glm::vec3(0.05f, 0.05f, 0.05f);
	dirLight.specular[2] = glm::vec3(0.2f, 0.2f, 0.2f);

	//BIOLAB
	dirLight.direction[3] = glm::vec3(-0.2f, -1.0f, -0.3f);
	dirLight.ambient[3] = glm::vec3(0.5f, 0.5f, 0.5f);
	dirLight.diffuse[3] = glm::vec3(1.0f, 1.0f, 1.0f);
	dirLight.specular[3] = glm::vec3(1.0f, 1.0f, 1.0f);

	//Point light
	for (int i = 0; i < ANZ_POINTLIGHTS; i++) {
		//lightcolor
		pointLight[i].lightColor[0] = glm::vec3(1, 0.89, 0.39); //DESERT
		pointLight[i].lightColor[1] = glm::vec3(0.22f, 0.35f, 0.53f);	//FACTORY
		pointLight[i].lightColor[2] = glm::vec3(0.84f, 0.0f, 0.0f);	//HORROR
		pointLight[i].lightColor[3] = glm::vec3(0.35f, 0.67f, 0.38f);	//BIOLAB

		//DESERT
		pointLight[i].ambient[0] = pointLight[i].lightColor[0] * glm::vec3(0.1f);
		pointLight[i].diffuse[0] = pointLight[i].lightColor[0];
		pointLight[i].specular[0] = pointLight[i].lightColor[0];

		pointLight[i].constant[0] = 1.0f;
		pointLight[i].linear[0] = 0.09f;
		pointLight[i].quadratic[0] = 0.032f;

		//FACTORY
		pointLight[i].ambient[1] = pointLight[i].lightColor[1] * glm::vec3(0.1f);
		pointLight[i].diffuse[1] = pointLight[i].lightColor[1];
		pointLight[i].specular[1] = pointLight[i].lightColor[1];

		pointLight[i].constant[1] = 1.0f;
		pointLight[i].linear[1] = 0.09f;
		pointLight[i].quadratic[1] = 0.032f;

		//HORROR
		pointLight[i].ambient[2] = pointLight[i].lightColor[2] * glm::vec3(0.1f);
		pointLight[i].diffuse[2] = pointLight[i].lightColor[2] * glm::vec3(lightIntensity);
		pointLight[i].specular[2] = pointLight[i].lightColor[2] * glm::vec3(lightIntensity);

		pointLight[i].constant[2] = 1.0f;
		pointLight[i].linear[2] = 0.14f;
		pointLight[i].quadratic[2] = 0.07f;

		//BIOLAB
		pointLight[i].ambient[3] = pointLight[i].lightColor[3] * glm::vec3(0.1f);
		pointLight[i].diffuse[3] = pointLight[i].lightColor[3];
		pointLight[i].specular[3] = pointLight[i].lightColor[3];

		pointLight[i].constant[3] = 1.0f;
		pointLight[i].linear[3] = 0.07f;
		pointLight[i].quadratic[3] = 0.017f;
	}


}











//Handler und Zeugs--------------------------------------------------------------------------------------------


void input()
{
	processInput(window, lightingShader);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
}

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
	updateCamera();

	
	//spotlight
	updateSpotlight();

	//ambiente
	updateAmbiente();

	//pointlight position
	updateLightPosition();



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

void updateSpotlight() {
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		if (spotlight && spotlightOn)
		{
			spotlight = false;
			oldCutOff = cutOff;
			oldOuterCutOff = outerCutOff;
			cutOff = 0.0f;
			outerCutOff = 0.0f;
			spotlightOff = true;
			spotlightOn = false;
		}
		else if (spotlight && spotlightOff)
		{
			spotlight = false;
			cutOff = oldCutOff;
			outerCutOff = oldOuterCutOff;
			spotlightOff = false;
			spotlightOn = true;
		}
	}
	else
	{
		spotlight = true;
	}
}

void updateAmbiente() {
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
	{
		atmosphere = DESERT;
		cutOff = 12.5f;
		outerCutOff = 13.0f;
		oldCutOff = cutOff;
		oldOuterCutOff = outerCutOff;
	}
	else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		atmosphere = FACTORY;
		cutOff = 10.0f;
		outerCutOff = 12.5f;
		oldCutOff = cutOff;
		oldOuterCutOff = outerCutOff;
	}
	else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		atmosphere = HORROR;
		cutOff = 10.0f;
		outerCutOff = 12.5f;
		oldCutOff = cutOff;
		oldOuterCutOff = outerCutOff;
	}
	else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
	{
		atmosphere = BIOLAB;
		cutOff = 7.0f;
		outerCutOff = 10.0f;
		oldCutOff = cutOff;
		oldOuterCutOff = outerCutOff;

	}
}

void updateLightPosition() {
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS){
		pointlightChoice = 0;
	} 
	else if(glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS){
	pointlightChoice = 1;
	}
	else if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS) {
		pointlightChoice = 2;
	}
	else if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS) {
		pointlightChoice = 3;
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		pointLightPositions[pointlightChoice].z -= deltaTime*2;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		pointLightPositions[pointlightChoice].z += deltaTime * 2;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		pointLightPositions[pointlightChoice].x -= deltaTime * 2;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		pointLightPositions[pointlightChoice].x += deltaTime * 2;
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		pointLightPositions[pointlightChoice].y -= deltaTime * 2;
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		pointLightPositions[pointlightChoice].y += deltaTime * 2;
	}

	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		if (!lightOffPressed)
		{
			oldQuadratic = pointLight[pointlightChoice].quadratic[atmosphere];
			oldColor = pointLight[pointlightChoice].lightColor[atmosphere];
			pointLight[pointlightChoice].quadratic[atmosphere] = 100000;
			pointLight[pointlightChoice].lightColor[atmosphere] = glm::vec3(0.0f);
		}
		lightOffPressed = true;
	}

	else if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
		pointLight[pointlightChoice].quadratic[atmosphere] = oldQuadratic;
		pointLight[pointlightChoice].lightColor[atmosphere] = oldColor;
		lightOffPressed = false;
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

