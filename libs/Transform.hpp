#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Component.hpp";
#include "shader.hpp";

class Transform : public Component {
private:
	Shader* shader;
public:
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;
	
	Transform(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) { 
		this->position = position;
		this->scale = scale;
		this->rotation = rotation;
	}

	void onUpdate() {
		glm::mat4 model = glm::mat4(1.0f);
		model = translate(model, position);
		model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, scale);
		int modelLoc = glGetUniformLocation(shader->getId(), "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	}

	void onGetOtherComponent(Component* component) {
		storeIfIsInstance(&shader, component);
	}

};