#pragma once

#include <vector>

#include "Texture.hpp"
#include "Material.hpp"
#include "Transform.hpp"
#include "Shader.hpp"
#include "Model.hpp"

class GameObject {
private:
	Model* model;
	Material* material;
	Texture* texture;
	vector<Transform*> instances;
public:	
	GameObject(Model* model, Material* material, Texture* texture) {
		this->model = model;
		this->material = material;
		this->texture = texture;
	}

	void Instantiate(Transform* transform) {
		instances.push_back(transform);
	}

	void Update(Shader* shader) {
		glEnable(GL_CULL_FACE);
		shader->setBool("reverse_normals", false);
		texture->onUpdate(shader);
		material->onUpdate(shader);
		for (Transform* transform : instances) {
			transform->onUpdate(shader);
			model->onDraw();
		}
	}
};