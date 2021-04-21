#pragma once
#include <glm/glm.hpp>

#include "Camera.hpp"
#include "Shader.hpp"

using namespace glm;

class Material {
private:
	Shader* shader;
	vec3 diffuse;
	vec3 specular;
	float shiniess;
	GameState* gs;
	Camera* cam;
public:
	Material(Shader* shader) {
		this->shader = shader;
		this->diffuse = vec3(1.0f, 0.5f, 0.31f);
		this->specular = vec3(0.5f, 0.5f, 0.5f);
		this->shiniess = 8.0f;
	}

	Material(Shader* shader, vec3 diffuse, vec3 specular, float shiniess) {
		this->shader = shader;
		this->diffuse = diffuse;
		this->specular = specular;
		this->shiniess = shiniess;
	}

	void setShader(Shader* shader) {
		this->shader = shader;
	}

	Shader* getShader() {
		return this->shader;
	}

	void onUpdate() {
		GameState* gs = GameState::get();
		Camera* cam = (*gs->getCamera());
		glm::mat4 view = (*GameState::get()->getCamera())->GetViewMatrix();
		glm::mat4 projection = glm::perspective(
			glm::radians(cam->Zoom),
			(float)gs->SCR_WIDTH / (float)gs->SCR_HEIGHT,
			0.1f,
			100.0f
		);
		shader->use();
		shader->setVec3("material.diffuse", diffuse);
		shader->setVec3("material.specular", specular);
		shader->setFloat("material.shininess", shiniess);
		shader->setVec3("ambient.color", 0.5f, 0.5f, 1.0f);
		shader->setFloat("ambient.brightness", 1.0f);
		shader->setFloat("ambient.min", 0.7f);
		shader->setFloat("ambient.max", 0.7f);
		shader->setVec3("viewPos", cam->Position);
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);		
	}
};