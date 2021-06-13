#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include "Shader.hpp"

using namespace glm;

class Light {
protected:
	vec3 position;
public:
	Light(vec3 position) {
		this->position = position;
	}

	void run(int index, Shader* shader) {
		feed(std::to_string(index), shader);
	}

	vec3 getPosition() {
		return position;
	}

	void setPosition(vec3 position) {
		this->position = position;
	}

	void Update(Shader* shader) {
		GameState* gs = GameState::get();
		Camera* cam = (*gs->getCamera());
		glm::mat4 view = (*GameState::get()->getCamera())->GetViewMatrix();
		glm::mat4 projection = glm::perspective(
			glm::radians(cam->Zoom),
			(float)gs->SCR_WIDTH / (float)gs->SCR_HEIGHT,
			0.1f,
			100.0f
		);
		glm::mat4 model = glm::mat4(1.0f);
		model = translate(model, getPosition());
		model = glm::scale(model, vec3(0.1f));
		shader->setMat4("model", model);
		shader->setVec3("viewPos", cam->Position);
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);
	}
protected:
	virtual void feed(std::string index, Shader* shader) {}
};