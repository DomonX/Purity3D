#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>
#include <string>

#include "libs/shader.hpp"
#include "libs/stb_image.h"
#include "libs/Camera.hpp"
#include "libs/Texture.hpp"
#include "libs/Transform.hpp"
#include "libs/GameState.hpp"
#include "libs/Time.hpp"
#include "libs/PointLight.hpp"
#include "libs/ObjLoader.hpp"
#include "libs/Model.hpp"
#include "libs/Material.hpp"

#include "libs/GameObject.hpp"
#include "libs/Skybox.hpp"

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

float deltaTime = 0.0f;
float lastFrame = 0.0f;

vec3 wallPos;

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
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		wallPos.x += 1.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		wallPos.x -= 1.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		wallPos.z += 1.0f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		wallPos.z -= 1.0f * deltaTime;
}

void init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

Transform* createTransform(int x, int y, int z) {
	return new Transform(vec3(2.0f * x, 2.0f * y, 2.0f * z), vec3(1.0f, 1.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f));
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
	glDepthFunc(GL_LESS);

	glEnable(GL_BLEND);

	glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);

	stbi_set_flip_vertically_on_load(true);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth cubemap texture
	unsigned int depthCubemap;
	glGenTextures(1, &depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (unsigned int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	gs = GameState::get();
	cam = gs->getCamera();

	Shader* cubeShader = new Shader("shaders/cube.vs", "shaders/cube.fs");
	cubeShader->use();
	cubeShader->setInt("texture1", 0);
	cubeShader->setInt("depthMap", 1);
	Shader* depthShader = new Shader("shaders/depth.vs.glsl", "shaders/depth.fs.glsl", "shaders/depth.gs.glsl");

	Shader* lightShader = new Shader("shaders/cube.vs", "shaders/light.fs");

	Material* material = new Material();

	Model* cube = new Model("assets/cube.obj");
	Model* sphere = new Model("assets/sphere.obj");

	Texture *tex = new Texture("assets/brick.jpg");

	vector<PointLight*> lights;
	vec3 lightPosition = vec3(1.0f, 6.0f, 1.0f);
	PointLight* light = new PointLight(lightPosition);
	light->brightness = 5.0f;

	wallPos = vec3(0.0f, 6.5f, 0.0f);

	GameObject* cubeObject = new GameObject(cube, material, tex);
	cubeObject->Instantiate(new Transform(vec3(1.0f), vec3(1.0f), vec3(0.0f)));
	cubeObject->Instantiate(new Transform(vec3(1.8f, 1.5f, 4.0f), vec3(1.0f), vec3(0.0f)));
	cubeObject->Instantiate(new Transform(wallPos, vec3(1.5f, 1.0f, 0.8f), vec3(0.0f)));

	Skybox* sky = new Skybox(cube, material, tex, new Transform(vec3(0.0f), vec3(10.0f), vec3(0.0f)));

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		Time::get()->setDeltaTime(deltaTime);

		processInput(window);

		// 0. create depth cubemap transformation matrices
		// -----------------------------------------------
		float near_plane = 0.01f;
		float far_plane = 100.0f;
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
		std::vector<glm::mat4> shadowTransforms;
		vec3 lightPos = light->getPosition();
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

		light->setPosition(lightPosition + vec3(sin(glfwGetTime()), 0.0, 0.0));

		(*cubeObject->Instance(0))->rotation = vec3(glfwGetTime(), 0.0, 0.0);
		(*cubeObject->Instance(2))->setPosition(wallPos);

		// 1. render scene to depth cubemap
		// --------------------------------
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		depthShader->use();
		for (unsigned int i = 0; i < 6; ++i)
			depthShader->setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
		depthShader->setFloat("far_plane", far_plane);
		depthShader->setVec3("lightPos", lightPos);
		cubeObject->Update(depthShader);
		sky->Update(depthShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// ---- Render scene ------
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		cubeShader->use();
		cubeShader->setFloat("far_plane", far_plane);

		// Setup lights
		lightShader->use();
		light->Update(lightShader);
		sphere->onDraw();

		cubeShader->use();
		cubeShader->setInt("pointLightsCounter", 1);
		light->run(0, cubeShader);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		
		cubeObject->Update(cubeShader);
		sky->Update(cubeShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
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