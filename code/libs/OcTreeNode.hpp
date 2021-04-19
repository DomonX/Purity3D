#pragma once

#include <glm/glm.hpp>
#include <boost/foreach.hpp>
#include "GameObject.hpp"
#include "Light.hpp"
#include "Transform.hpp"
#include "Shader.hpp"

#define OC_RIGHT 0
#define OC_UP 0
#define OC_FORWARD 0
#define OC_LEFT 1
#define OC_DOWN 2
#define OC_BACKWARD 4

#define MAX_OBJ 32

#define MAX_LIGHTS 8

Model* model = nullptr;

class OcTreeNode {
public:
	OcTreeNode* parent;
	OcTreeNode* children[8];
	bool hasBeenPartified = false;
	bool isVisible = true;

	Shader* shader = new Shader("shaders/cubeVs.glsl", "shaders/ocTreeFs.glsl");
	
	glm::vec3 size;
	glm::vec3 position;

	vector<GameObject*> gameObjects;
	vector<Light*> lights;

	int depth = 0;
public:
	OcTreeNode(glm::vec3 position, glm::vec3 size) {
		if (model == nullptr) {
			model = new Model("assets/ocTreeCube.obj");
		}
		model->setDrawMode(GL_LINES);
		this->position = position;
		this->size = size;
		parent = nullptr;
	}

	OcTreeNode(OcTreeNode* parent, glm::vec3 position, glm::vec3 size) {
		if (model == nullptr) {
			model = new Model("assets/ocTreeCube.obj");
		}
		model->setDrawMode(GL_LINES);
		this->position = position;
		this->size = size;
		this->parent = parent;
		this->depth = parent->depth + 1;
		getElementsFromParent();
	}

	void addLight(Light* light) {
		lights.push_back(light);
		if (hasBeenPartified) {
			return;
		}
		if (lights.size() > MAX_LIGHTS) {
			partify();
		}
	}

	void addObject(GameObject* object) {
		gameObjects.push_back(object);
		if (hasBeenPartified) {
			return;
		}
		if (gameObjects.size() > MAX_OBJ) {
			partify();
		}
	}

	void onUpdate() {
		if (isVisible) {
			shader->onUpdate();
			draw();
			model->onDraw();
		}
		for (OcTreeNode* node : children) {
			if (node) {
				node->onUpdate();
			}
		}
	}

public: // To be Private

	void wrap() {

	}

	void partify() {
		for (int i = 0; i < 8; i++) {
			children[i] = new OcTreeNode(this, position + getShift(i), size * vec3(0.5f));
		}
	}

	void getElementsFromParent() {
		for (GameObject* go : parent->gameObjects) {
			addGameObjectIfInside(go);
		}

		for (Light* l : parent->lights) {
			addLightIfInside(l);
		}
	}

	bool isInside(vec3 point) {

		if (!inRange(point.x, position.x - size.x / 2, position.x + size.x / 2)) {
			return false;
		}

		if (!inRange(point.y, position.y - size.y / 2, position.y + size.y / 2)) {
			return false;
		}

		if (!inRange(point.z, position.z - size.z / 2, position.z + size.z / 2)) {
			return false;
		}

		return true;

	}

	bool inRange(float pos, float min, float max) {
		return pos > min && pos <= max;
	}

	void draw() {
		glm::mat4 model = glm::mat4(1.0f);
		model = translate(model, position);
		model = glm::scale(model, size);
		int modelLoc = glGetUniformLocation(shader->getId(), "model");
		if (depth == 0) {
			shader->setVec3("Color", vec3(0.0f, 0.0f, 1.0f));
		}
		if (depth == 1) {
			shader->setVec3("Color", vec3(1.0f, 0.0f, 0.0f));
		}
		if (depth == 2) {
			shader->setVec3("Color", vec3(0.0f, 1.0f, 1.0f));
		}
		if (depth == 3) {
			shader->setVec3("Color", vec3(1.0f, 1.0f, 0.0f));
		}
		if (depth > 3) {
			shader->setVec3("Color", vec3(1.0f, 1.0f, 1.0f));
		}
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	}

	void addGameObjectIfInside(GameObject* go) {
		Transform* tr = go->getComponent<Transform>();
		if (!tr) {
			return;
		}
		if (isInside(tr->position)) {
			addObject(go);
		}
	}

	void addLightIfInside(Light* l) {
		if (isInside(l->getPosition())) {
			addLight(l);
		}
	}

	vec3 getShift(unsigned int childPos) {
		vec3 shift = vec3(0.0f);
		if (childPos & OC_LEFT) {
			shift -= vec3(size.x / 2, 0.0f, 0.0f);
		}
		else {
			shift += vec3(size.x / 2, 0.0f, 0.0f);
		}
		if (childPos & OC_DOWN) {
			shift -= vec3(0.0f, size.y / 2, 0.0f);
		}
		else {
			shift += vec3(0.0f, size.y / 2, 0.0f);
		}
		if (childPos & OC_BACKWARD) {
			shift -= vec3(0.0f, 0.0f, size.z / 2);
		}
		else {
			shift += vec3(0.0f, 0.0f, size.z / 2);
		}
		return shift;
	}

	OcTreeNode* getProperChildren(vec3 positon) {
		for (OcTreeNode* c : children) {
			if (c->isInside(position)) {
				return c;
			}
		}
	}

};