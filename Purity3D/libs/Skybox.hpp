#pragma once

#pragma once

#include "Texture.hpp"
#include "Material.hpp"
#include "Transform.hpp"
#include "Shader.hpp"
#include "Model.hpp"

class Skybox {
private:
	Model* model;
	Material* material;
	Texture* texture;
	Transform* transform;
public:
	Skybox(Model* model, Material* material, Texture* texture, Transform* transform) {
		this->model = model;
		this->material = material;
		this->texture = texture;
		this->transform = transform;
	}

	void Update(Shader* shader) {
		glDisable(GL_CULL_FACE);
		shader->setBool("reverse_normals", true);
		texture->onUpdate(shader);
		material->onUpdate(shader);
		transform->onUpdate(shader);
		model->onDraw();
	}
};