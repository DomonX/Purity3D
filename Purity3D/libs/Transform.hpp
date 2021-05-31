#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp";

using namespace glm;

class Transform {
private:
	Transform* parent = nullptr;
	glm::vec3 position;
public:
	glm::vec3 scale;
	glm::vec3 rotation;
	
	Transform(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) { 
		this->position = position;
		this->scale = scale;
		this->rotation = rotation;
	}

	void onUpdate(Shader* shader) {
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
		model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, scale);
		shader->setMat4("model", model);
		shader->setVec3("viewPos", cam->Position);
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);
	}

	glm::vec3 getPosition() {
		if (parent == nullptr) {
			return position;
		}
		return parent->getPosition() + position;
	}

	glm::vec3 getRawPosition() {
		return position;
	}

	void setPosition(vec3 position) {
		this->position = position;
	}

	void setParent(Transform* parent) {
		this->parent = parent;
	}

};