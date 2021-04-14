#pragma once

#include <vector>
#include "Component.hpp"
#include "Model.hpp"
#include "shader.hpp"

using namespace std;

class GameObject : public Component {
private:
	Model* model;
	Shader* shader;
	vector<Component*> components;
	vector<GameObject*> children;
public:
	GameObject(Model* model, Shader* shader) {
		this->model = model;
		this->shader = shader;
	}

	void addComponent(Component* component) {
		component->getGameObject(this);
		for (Component* i : components) {
			component->onGetOtherComponent(i);
			i->onGetOtherComponent(component);
		}
		for (GameObject* i : children) {
			i->onGetParentComponent(component);
		}
		components.push_back(component);		
		component->onStart();
	}

	void onStart() {
		for (Component* i : components) {
			i->onStart();
		}
	}

	void onUpdate() {
		shader->onUpdate();
		for (Component* i : components) {
			i->onUpdate();
		}
		for (Component* i : components) {
			i->onDraw();
		}
		model->onDraw();

	}

	void onDelete() {
		for (Component* i : components) {
			i->onDelete();
		}
		for (Component* i : components) {
			delete(i);
		}
		components.clear();
	}

	template<class T> T* getComponent() {
		T* temp = nullptr;
		for (Component* component : components) {
			temp = dynamic_cast<T*>(component);
			if (temp) {
				return temp;
			}
		}
		return temp;
	}

	Shader * getShader() {
		return shader;
	}

};