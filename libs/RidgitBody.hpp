#pragma once

#include "Transform.hpp"
#include "Component.hpp"
#include "Time.hpp"

using namespace glm;

class RidgitBody : public Component {

	glm::vec3 velocity = glm::vec3(0.0f);
	Transform* transform = nullptr;
	float elasticity = 0.0f;

public:

	RidgitBody() {

	}

	RidgitBody(float elasticity) {
		this->elasticity = elasticity;
	}

	void onGetOtherComponent(Component* component) {
		storeIfIsInstance(&transform, component);
	}

	void addForce(glm::vec3 force) {
		velocity += force;
		//std::cout << velocity.x << "." << velocity.y << "." << velocity.z << endl;
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

	void reflect(vec3 norm, vec3 newForce) {
		
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