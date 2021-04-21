#pragma once

#include "Transform.hpp"
#include "Component.hpp"
#include "Time.hpp"

using namespace glm;

class RidgitBody : public Component {

	glm::vec3 velocity = glm::vec3(0.0f);
	Transform* transform = nullptr;
	float elasticity;

public:

	RidgitBody() {
		this->elasticity = 0.0f;
	}

	RidgitBody(float elasticity) {
		this->elasticity = elasticity;
	}

	void onGetOtherComponent(Component* component) {
		storeIfIsInstance(&transform, component);
	}

	void addForce(glm::vec3 force) {
		velocity += force;
	}

	glm::vec3 getForce() {
		return velocity;
	}

	void resetForce() {
		velocity = glm::vec3(0.0f);
	}

	vec3 getSelfVector() {
		return velocity * elasticity;
	}

	vec3 getReflectionVector(vec3 norm) {
		return (velocity * norm) * vec3(-1 + elasticity);
	}

	void setForce(vec3 force) {
		velocity = force;
	}

	void onUpdate() {
		(*transform).position = transform->position + (velocity * Time::getDeltaTime());
	}

	Transform* getTransfrom() {
		return transform;
	}
};