#pragma once

#include "Light.hpp"

using namespace glm;

class PointLight : public Light {

public:
	PointLight(vec3 position) : Light(position) { }
	vec3 diffuse = vec3(0.5f, 0.5f, 0.5f);
	vec3 specular = vec3(1.0f, 1.0f, 1.0f);
	vec3 color = vec3(1.0f, 1.0f, 1.0f);
	float constant = 1.0f;
	float linear = 0.09f;
	float quadric = 0.032f;
	float brightness = 2.0f;

	void feed(std::string index, Shader* shader) {
		std::string variableName = "pointLights[" + index + "]";
		shader->setVec3(variableName + ".position", position);
		shader->setVec3(variableName + ".diffuse", diffuse);
		shader->setVec3(variableName + ".specular", specular);
		shader->setVec3(variableName + ".color", color);
		shader->setFloat(variableName + ".constant", constant);
		shader->setFloat(variableName + ".linear", linear);
		shader->setFloat(variableName + ".quadratic", quadric);
		shader->setFloat(variableName + ".brightness", brightness);
	}

	void setColor(vec3 color) {
		this->color = color;
	}
};