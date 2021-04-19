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
protected:
	virtual void feed(std::string index, Shader* shader) {}
};