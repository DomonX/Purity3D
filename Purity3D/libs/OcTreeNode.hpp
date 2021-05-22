#pragma once

#include <glm/glm.hpp>
#include <boost/foreach.hpp>
#include "GameObject.hpp"
#include "Light.hpp"
#include "Transform.hpp"
#include "Shader.hpp"
#include "MotionListener.hpp"

#define OC_RIGHT 0
#define OC_UP 0
#define OC_FORWARD 0
#define OC_LEFT 1
#define OC_DOWN 2
#define OC_BACKWARD 4

Model* model = nullptr;

class OcTreeNode : public MotionListener {
public:
	OcTreeNode* parent;
	OcTreeNode* children[8];
	bool hasBeenPartified = false;
	bool isVisible = true;

	glm::vec3 size;
	glm::vec3 position;

	vector<GameObject*> gameObjects;

	int depth = 0;
public:
	OcTreeNode(glm::vec3 position, glm::vec3 size) {
		if (model == nullptr) {
			model = createModel();
			model->setDrawMode(GL_LINES);
		}
		this->position = position;
		this->size = size;
		parent = nullptr;
	}

	OcTreeNode(OcTreeNode* parent, glm::vec3 position, glm::vec3 size) {
		if (model == nullptr) {
			model = createModel();
			model->setDrawMode(GL_LINES);
		}
		this->position = position;
		this->size = size;
		this->parent = parent;
		this->depth = parent->depth + 1;
	}

	void addObject(GameObject* object) {
		vec3 pos = getPositionOfGameObject(object);
		if (!isInside(pos)) {
			return;
		}
		if (!hasBeenPartified) {
			object->setListener(this);
			gameObjects.push_back(object);
			return;
		}
		OcTreeNode* node = getProperChildren(pos);
		if (node) {
			node->addObject(object);
		}
	}

	void onUpdate() {
		if (isVisible) {
			model->onUpdate();
			draw();
			model->onDraw();
		}
		for (OcTreeNode* node : children) {
			if (node) {
				node->onUpdate();
			}
		}
		if (!hasBeenPartified) {
			for (GameObject* go : gameObjects) {
				go->onUpdate();
			}
		}
	}

	static Model* createModel() {
		Shader* shader = new Shader("shaders/cube.vs", "shaders/ocTree.fs");
		Material* material = new Material(shader);
		return new Model("assets/ocTreeCube.obj", material);
	}

	Shader* getShader() {
		return model->getMaterial()->getShader();
	}

public:
	void partify() {
		for (int i = 0; i < 8; i++) {
			children[i] = new OcTreeNode(this, position + getShift(i), size * vec3(0.5f));
		}
		for (GameObject* go : gameObjects) {
			getProperChildren(getPositionOfGameObject(go))->addObject(go);
		}
		gameObjects.clear();
		hasBeenPartified = true;
	}

	vec3 getPositionOfGameObject(GameObject* go) {
		Transform* tr = go->getComponent<Transform>();
		if (!tr) {
			return vec3(0.0f, 0.0f, 0.0f);
		}
		return tr->getPosition();
	}

	bool isInside(vec3 point) {
		if (!inRange(point.x, position.x - size.x, position.x + size.x)) {
			return false;
		}
		if (!inRange(point.y, position.y - size.y, position.y + size.y)) {
			return false;
		}
		if (!inRange(point.z, position.z - size.z, position.z + size.z)) {
			return false;
		}
		return true;
	}

	bool inRange(float pos, float min, float max) {
		return pos > min && pos <= max;
	}

	void draw() {
		if (hasBeenPartified) {
			return;
		}
		glm::mat4 model = glm::mat4(1.0f);
		model = translate(model, position);
		model = glm::scale(model, size);
		if (gameObjects.size() > 0) {
			getShader()->setVec3("Color", vec3(0.0f, 0.0f, 1.0f));
		} else {
			getShader()->setVec3("Color", vec3(1.0f, 0.0f, 0.0f));
		}
		getShader()->setMat4("model", model);
	}

	vec3 getShift(unsigned int childPos) {
		vec3 shift = vec3(0.0f);
		if (childPos & OC_LEFT) {
			shift -= vec3(size.x / 2, 0.0f, 0.0f);
		} else {
			shift += vec3(size.x / 2, 0.0f, 0.0f);
		}
		if (childPos & OC_DOWN) {
			shift -= vec3(0.0f, size.y / 2, 0.0f);
		} else {
			shift += vec3(0.0f, size.y / 2, 0.0f);
		}
		if (childPos & OC_BACKWARD) {
			shift -= vec3(0.0f, 0.0f, size.z / 2);
		} else {
			shift += vec3(0.0f, 0.0f, size.z / 2);
		}
		return shift;
	}

	OcTreeNode* getProperChildren(vec3 position) {
		for (OcTreeNode* c : children) {
			if (c->isInside(position)) {
				return c;
			}
		}
		throw "Unable to find children of OcTree Node when it should be";
	}

	void objectMoved(Component* object, vec3 dest) {
		if (isInside(dest)) {
			return;
		}
		GameObject* go = dynamic_cast<GameObject*>(object);
		gameObjects.erase(remove(gameObjects.begin(), gameObjects.end(), go), gameObjects.end());
		transferObject(go, dest);
	}

	void transferObject(GameObject* object, vec3 dest) {
		bool inside = isInside(dest);
		if (!inside) {
			if (!parent) {
				return;
			}
			parent->transferObject(object, dest);
			return;
		}
		if (!hasBeenPartified) {
			addObject(object);
			return;
		}
		if (hasBeenPartified) {
			getProperChildren(dest)->addObject(object);
		}
	}

};