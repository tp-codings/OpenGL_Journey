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
Camera camera(glm::vec3(0.0f, 45.0f, 235.0f));
float lastMouseX = WINDOW_WIDTH / 2.0f, lastMouseY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;

//Shader--------------------------------------------------------------------------------------------

Shader asteroidsShader;
Shader planetShader;
Shader screenShader;


//Vertex Shader
const char* defaultVertexPath = "D:\\1. Programmieren\\OpenGL\\Programme\\asteroids\\default.vs";
const char* screenVertexPath = "D:\\1. Programmieren\\OpenGL\\Programme\\asteroids\\screen.vs";
const char* planetVertexPath = "D:\\1. Programmieren\\OpenGL\\Programme\\asteroids\\planet.vs";


//Fragment Shader
const char* defaultFragmentPath = "D:\\1. Programmieren\\OpenGL\\Programme\\asteroids\\default.fs";
const char* screenFragmentPath = "D:\\1. Programmieren\\OpenGL\\Programme\\asteroids\\screen.fs";
const char* planetFragmentPath = "D:\\1. Programmieren\\OpenGL\\Programme\\asteroids\\planet.fs";

//Geometry Shader

//Matrizen--------------------------------------------------------------------------------------------
glm::mat4 projection;
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 model = glm::mat4(1.0f);
glm::vec3 cubePosition = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cubePosition2 = glm::vec3(0.0f, 4.0f, 0.0f);

//Worldobjekte--------------------------------------------------------------------------------------------

Model asteroid;
Model planet;

float screenVertices[] = {
	// positions // texCoords
	-1.0f, 1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 1.0f, 0.0f,

	-1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f
};



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
void processInput(GLFWwindow* window, Shader asteroidsShader);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
unsigned int loadTexture(char const* path);
unsigned int loadCubeMap(vector<std::string> faces);


int choice = 0;

unsigned int amount = 100000;
glm::mat4* modelMatrices;
float radius = 200.0;
float offset = 60.0f;
//--------------------------------------------------------------------------------------------


int main()
{
	modelMatrices = new glm::mat4[amount];
	srand(static_cast<unsigned int>(glfwGetTime())); // initialize random seed

	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.1f; // keep height of asteroid field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 2. scale: Scale between 0.05 and 0.25f
		float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
		model = glm::scale(model, glm::vec3(scale));

		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		float rotAngle = static_cast<float>((rand() % 360));
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. now add to list of matrices
		modelMatrices[i] = model;
	}
	//WINDOW--------------------------------------------------------------------------------------------
	initWindow();

	//BUFFER ----------------------------------------------------------------------------------------------------



	//SHADER ----------------------------------------------------------------------------------------------------
	initShader();


	//Textures--------------------------------------------------------------------------------------------
	initTextures();

	//Models
	stbi_set_flip_vertically_on_load(true);
	asteroid = Model("D:\\1. Programmieren\\OpenGL\\resources\\models\\rock\\rock.obj");
	planet = Model("D:\\1. Programmieren\\OpenGL\\resources\\models\\planet\\planet.obj");
	initBuffer();
	//Tests--------------------------------------------------------------------------------------------
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
	projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);
	view = camera.GetViewMatrix();

	asteroidsShader.use();
	asteroidsShader.setMat4("projection", projection);
	asteroidsShader.setMat4("view", view);

	planetShader.use();
	planetShader.setMat4("projection", projection);
	planetShader.setMat4("view", view);

	// draw planet
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	planetShader.setMat4("model", model);
	planet.Draw(planetShader);


	// draw meteorites
	asteroidsShader.use();
	asteroidsShader.setInt("texture_diffuse1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, asteroid.textures_loaded[0].id); // note: we also made the textures_loaded vector public (instead of private) from the model class.


	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.1f; // keep height of asteroid field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 2. scale: Scale between 0.05 and 0.25f
		float scale = static_cast<float>((rand() % 20) / 100.0 + 0.05);
		model = glm::scale(model, glm::vec3(scale));

		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		float rotAngle = static_cast<float>(glfwGetTime());
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. now add to list of matrices
		modelMatrices[i] = model;
	}
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, amount * sizeof(glm::mat4), &modelMatrices[0]);


	for (unsigned int i = 0; i < asteroid.meshes.size(); i++)
	{
		glBindVertexArray(asteroid.meshes[i].VAO);
		glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(asteroid.meshes[i].indices.size()), GL_UNSIGNED_INT, 0, amount);
		glBindVertexArray(0);
	}


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
	// configure instanced array
	// -------------------------
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	// set transformation matrices as an instance vertex attribute (with divisor 1)
	// note: we're cheating a little by taking the, now publicly declared, VAO of the model's mesh(es) and adding new vertexAttribPointers
	// normally you'd want to do this in a more organized fashion, but for learning purposes this will do.
	// -----------------------------------------------------------------------------------------------------------------------------------
	for (unsigned int i = 0; i < asteroid.meshes.size(); i++)
	{
		unsigned int VAO = asteroid.meshes[i].VAO;
		glBindVertexArray(VAO);
		// set attribute pointers for matrix (4 times vec4)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}

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
	asteroidsShader = Shader(defaultVertexPath, defaultFragmentPath);

	screenShader = Shader(screenVertexPath, screenFragmentPath);

	planetShader = Shader(planetVertexPath, planetFragmentPath);
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
	processInput(window, asteroidsShader);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window, Shader asteroidsShader)
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

