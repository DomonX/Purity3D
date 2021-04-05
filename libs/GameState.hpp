#pragma once

#include "Camera.hpp"

class GameState {
private:
	static GameState* instance;
	static Camera* cam;
	GameState() {
		cam = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	}
public:
	static const unsigned int SCR_WIDTH = 1920;
	static const unsigned int SCR_HEIGHT = 1080;

	static GameState* get() {
		if (instance) {
			return instance;
		}
		instance = new GameState();
	}

	Camera** getCamera() {
		return &cam;
	}
};