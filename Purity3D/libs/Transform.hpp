#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GameObject.hpp";
#include "shader.hpp";

class Transform : public Component {
private:
	GameObject* go = nullptr;
	Transform* parent = nullptr;
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
		model = translate(model, getPosition());
		model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, scale);
		int modelLoc = glGetUniformLocation(go->getMaterial()->getShader()->getId(), "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	}

	void getGameObject(Component* go) {
		this->go = (GameObject*)go;
	}

	glm::vec3 getPosition() {
		if (parent == nullptr) {
			return position;
		}
		return parent->getPosition() + position;
	}

	void setParent(Transform* parent) {
		this->parent = parent;
	}

};