#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

#include "libs/shader.hpp"
#include "libs/stb_image.h"
#include "libs/Cube.hpp"
#include "libs/Camera.hpp"
#include "libs/Texture.hpp"
#include "libs/Transform.hpp"
#include "libs/GameObject.hpp"
#include "libs/GameState.hpp"
#include "libs/RotatorContoller.hpp"
#include "libs/Time.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

using namespace std;
using namespace glm;

GameState* gs;
Camera** cam;
float lastX;
float lastY;
bool firstMouse = true;

Cube* c;
Shader* currentShader;
Texture* tex;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		(*cam)->ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		(*cam)->ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		(*cam)->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		(*cam)->ProcessKeyboard(RIGHT, deltaTime);
}

void init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

Transform* createTransform(int x, int y, int z, bool wall, int wallMode) {
	float height = wall ? 1.0f : 0.1f;
	float heightShift = wall ? 0.0f : 0.45f;
	float w1 = wall && wallMode == 0 || wallMode == 2 || wallMode == 4 || wallMode == 5 ? 0.1f : 1.0f;
	float w2 = wall && wallMode == 1 || wallMode == 3 ? 0.1f : 1.0f;
	float wShift1 = 0.0f;
	float wShift2 = 0.0f;
	float angl = 0.0f;
	w2 = wallMode == 4 || wallMode == 5 ? 1.41f : w2;
	if (wallMode == 0) {
		wShift1 = -0.45f;
	}
	if (wallMode == 1) {
		wShift2 = 0.45f;
	}
	if (wallMode == 2) {
		wShift1 = -0.45f;
	}
	if (wallMode == 3) {
		wShift2 = -0.45f;
	}
	if (wallMode == 4) {
		angl = 0.85f;
	}

	if (wallMode == 5) {
		angl = -1.2f;
	}
	return new Transform(vec3(1.0f * x + wShift1, 1.0f * y + heightShift, 1.0f * z + wShift2), vec3(w1, height, w2), vec3(0.0f, angl, 0.0f));
}

GameObject* createCube(int x, int y, int z, bool wall, int wallMode) {
	GameObject* obj = new GameObject(c, currentShader);
	obj->addComponent(tex);
	obj->addComponent(createTransform(x, y, z, wall, wallMode));
	//obj->addComponent(new RotatorController(glm::vec3(0.2f + i * 1.1, 0.2f + j * 1.1, 0.2f)));
	return obj;
}

int main() {
	init();
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(-1);
	}
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glEnable(GL_DEPTH_TEST);

	stbi_set_flip_vertically_on_load(true);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);

	gs = GameState::get();
	cam = gs->getCamera();

	float lastX = gs->SCR_WIDTH / 2.0f;
	float lastY = gs->SCR_HEIGHT / 2.0f;

	c = new Cube();

	tex = new Texture("container2.png");
	currentShader = new Shader("cubeVs.glsl", "cubeFs.glsl");

	vector<GameObject*> objs;

	int sx = -10;
	int sy = -10;
	int ex = 10;
	int ey = 10;

	for (int i = sx; i < ex; i++) {
		for (int j = sy; j < ey; j++) {		
			objs.push_back(createCube(i, 0, j, false, -1));
		}
	}	


	objs.push_back(createCube(3, 1, 1, true, 4));
	objs.push_back(createCube(2, 1, 1, true, 1));
	objs.push_back(createCube(2, 1, 1, true, 3));
	objs.push_back(createCube(3, 1, 1, true, 1));

	Shader* lightShader = new Shader("cubeVs.glsl", "lightFs.glsl");
	GameObject* light = new GameObject(c, lightShader);
	Transform* lightTransform = new Transform(vec3(0.0f, 3.0f, 0.0f), vec3(0.2f), vec3(0.0f));
	light->addComponent(lightTransform);

	while (!glfwWindowShouldClose(window)) {

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		Time::get()->setDeltaTime(deltaTime);

		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (GameObject* obj : objs) {
			currentShader->setVec3("light.position", lightTransform->position);
			obj->onUpdate();
		}
		//lightShader->use();
		//lightTransform->position = lightTransform->position + vec3(0.0f, 0.01f, 0.0f);
		light->onUpdate();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	(*cam)->ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	(*cam)->ProcessMouseScroll(yoffset);
}
