#pragma once
#include <glm/glm.hpp>

#include "Camera.hpp"
#include "Shader.hpp"

using namespace glm;

class Material {
private:
	vec3 diffuse;
	vec3 specular;
	float shiniess;
	GameState* gs;
public:
	Material() {
		this->diffuse = vec3(1.0f, 0.5f, 0.31f);
		this->specular = vec3(0.5f, 0.5f, 0.5f);
		this->shiniess = 8.0f;
	}

	Material(vec3 diffuse, vec3 specular, float shiniess) {
		this->diffuse = diffuse;
		this->specular = specular;
		this->shiniess = shiniess;
	}

	void onUpdate(Shader* shader) {
		shader->setVec3("material.diffuse", diffuse);
		shader->setVec3("material.specular", specular);
		shader->setFloat("material.shininess", shiniess);
		shader->setVec3("ambient.color", 0.5f, 0.5f, 1.0f);
		shader->setFloat("ambient.brightness", 1.0f);
		shader->setFloat("ambient.min", 0.1f);
		shader->setFloat("ambient.max", 0.2f);
	}
};