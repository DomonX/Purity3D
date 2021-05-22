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
#include "libs/GameObject.hpp"
#include "libs/GameState.hpp"
#include "libs/RotatorContoller.hpp"
#include "libs/Time.hpp"
#include "libs/PointLight.hpp"
#include "libs/ObjLoader.hpp"
#include "libs/OcTreeNode.hpp"
#include "libs/RidgitBody.hpp"
#include "libs/SphereCollider.hpp"

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
Model* cube;
Model* sphere;
Shader* currentShader;
Material* material;
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

Transform* createTransform(int x, int y, int z) {
	return new Transform(vec3(2.0f * x, 2.0f * y, 2.0f * z), vec3(1.0f, 1.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f));
}

GameObject* createCube(int x, int y, int z) {
	GameObject* obj = new GameObject(cube);
	obj->addComponent(tex);
	obj->addComponent(createTransform(x, y, z));
	return obj;
}

GameObject* createSphere(float x, float y, float z, float elasticity = 0.0f) {
	GameObject* sphereO = new GameObject(sphere);
	sphereO->addComponent(tex);
	sphereO->addComponent(new Transform(vec3(2.0f * x, 2.0f * y, 2.0f * z), vec3(1.0f, 1.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f)));
	sphereO->addComponent(new RidgitBody(elasticity));
	sphereO->addComponent(new SphereCollider());
	return sphereO;
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

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);

	stbi_set_flip_vertically_on_load(true);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	gs = GameState::get();
	cam = gs->getCamera();

	currentShader = new Shader("shaders/cube.vs", "shaders/cube.fs");
	material = new Material(currentShader);

	cube = new Model("assets/cube.obj", material);
	sphere = new Model("assets/sphere.obj", material);

	tex = new Texture("assets/brick.jpg");

	vector<GameObject*> objs;

	vector<PointLight*> lights;

	PointLight* light = new PointLight(vec3(6.0f, 10.0f, 6.0f));

	lights.push_back(light);
	lights.push_back(new PointLight(vec3(2.0f, 3.0f, 1.0f)));
	lights.push_back(new PointLight(vec3(6.0f, 4.0f, 5.0f)));

	int sx = -10;
	int sy = -10;
	int ex = 10;
	int ey = 10;

	OcTreeNode* ocT = new OcTreeNode(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 10.0f, 10.0f));
	ocT->partify();
	ocT->children[OC_LEFT | OC_UP | OC_BACKWARD]->partify();
	ocT->children[OC_LEFT | OC_UP | OC_BACKWARD]->children[OC_LEFT | OC_DOWN | OC_FORWARD]->partify();

	for (int i = sx; i < ex; i++) {
		for (int j = sy; j < ey; j++) {	
			ocT->addObject(createCube(i, 0, j));
		}
	}	

	GameObject* cubeO = new GameObject(cube);
	cubeO->addComponent(tex);
	cubeO->addComponent(new Transform(vec3(2.0f, 2.5f, 1.5f), vec3(1.0f, 1.0f, 1.0f), vec3(0.0f, 0.0f, 0.0f)));
	RidgitBody* body = new RidgitBody();
	cubeO->addComponent(body);
	body->addForce(vec3(0.0f, 0.0f, -1.4f));
	ocT->addObject(cubeO);

	GameObject* sphere = createSphere(-7, 1.2, 1, 0.0f);
	RidgitBody* sBody = sphere->getComponent<RidgitBody>();
	SphereCollider* col = sphere->getComponent<SphereCollider>();
	sBody->addForce(vec3(5.51f, 0.0f, 0.0f));
	ocT->addObject(sphere);

	GameObject* sphere2 = createSphere(5, 1, 1, 0.2f);
	RidgitBody* sBody2 = sphere2->getComponent<RidgitBody>();
	SphereCollider* col2 = sphere2->getComponent<SphereCollider>();
	sBody2->addForce(vec3(-0.51f, 0.0f, 0.0f));
	ocT->addObject(sphere2);

	GameObject* sphere3 = createSphere(4, 1.3, 4.3, 0.4f);
	RidgitBody* sBody3 = sphere3->getComponent<RidgitBody>();
	SphereCollider* col3 = sphere3->getComponent<SphereCollider>();
	sBody3->addForce(vec3(0.0f, 0.0f, -0.90f));
	ocT->addObject(sphere3);

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		Time::get()->setDeltaTime(deltaTime);
		glClear(GL_DEPTH_BUFFER_BIT);

		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		currentShader->use();
		currentShader->setInt("pointLightsCounter", lights.size());
		float val = (sin(glfwGetTime()) + 1) / 2;
		light->setColor(vec3(0.0f, 1 - val, val));
		light->brightness = 2.0f + 3 * val;
		for (int i = 0; i < lights.size(); i++) {
			lights.at(i)->run(i, currentShader);
		}
		col->getCollision(col2);
		col->getCollision(col3);
		col2->getCollision(col3);
		ocT->onUpdate();
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