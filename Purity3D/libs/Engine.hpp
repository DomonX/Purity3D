#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "Time.hpp"
#include "Camera.hpp"
#include "GameState.hpp"
#include "stb_image.h"

#include "GameObject.hpp"
#include "Light.hpp"

using namespace std;

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

Camera** cam;
GameState* gs;
float lastX = 0;
float lastY = 0;

float lastFrame;

GLFWwindow* window;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		(*cam)->ProcessKeyboard(FORWARD, Time::getDeltaTime());
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		(*cam)->ProcessKeyboard(BACKWARD, Time::getDeltaTime());
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		(*cam)->ProcessKeyboard(LEFT, Time::getDeltaTime());
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		(*cam)->ProcessKeyboard(RIGHT, Time::getDeltaTime());
}

void mouse_cb(GLFWwindow* window, double xpos, double ypos)
{
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	(*cam)->ProcessMouseMovement(xoffset, yoffset);
}

void scroll_cb(GLFWwindow* window, double xoffset, double yoffset)
{
	(*cam)->ProcessMouseScroll(yoffset);
}

class Engine {
private:
	vector<GameObject*> objs;
	vector<Light*> lights;
public:
	void init() {
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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
		glfwSetCursorPosCallback(window, mouse_cb);
		glfwSetScrollCallback(window, scroll_cb);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		glEnable(GL_BLEND);

		stbi_set_flip_vertically_on_load(true);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		gs = GameState::get();
		cam = gs->getCamera();
	}

	void mainLoop() {
		float currentFrame = glfwGetTime();
		float deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		Time::get()->setDeltaTime(deltaTime);
		glClear(GL_DEPTH_BUFFER_BIT);

		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		for (int i = 0; i < lights.size(); i++) {
			// lights.at(i)->run(i, currentShader);
		}
		for (GameObject* obj : objs) {
			obj->onUpdate();
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	void start() {
		while (!glfwWindowShouldClose(window)) {
			float currentFrame = glfwGetTime();
			float deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			Time::get()->setDeltaTime(deltaTime);
			glClear(GL_DEPTH_BUFFER_BIT);

			processInput(window);

			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			currentShader->use();
			currentShader->setInt("pointLightsCounter", lights.size());
			for (int i = 0; i < lights.size(); i++) {
				lights.at(i)->run(i, currentShader);
			}
			for (GameObject* obj : objs) {
				obj->onUpdate();
			}
			col->getCollision(col2);
			col->getCollision(col3);
			col2->getCollision(col3);
			// ocT->onUpdate();
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glfwTerminate();
		return 0;
	}
};