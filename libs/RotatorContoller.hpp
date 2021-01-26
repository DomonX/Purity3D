#pragma once

#include "Component.hpp"
#include "Transform.hpp"
#include "Time.hpp"

class RotatorController : public Component {
private:
	glm::vec3 speed;
	Transform* transform = nullptr;
public:
	
	RotatorController(glm::vec3 speed) {
		this->speed = speed;
	}

	void onGetOtherComponent(Component* component) {
		storeIfIsInstance(&transform, component);
	}

	void onUpdate() {
		glm::vec3 rot = transform->rotation;
		rot.x += speed.x * Time::getDeltaTime();
		if (rot.x >= 360) {
			rot.x -= 360;
		}
		rot.y += speed.y * Time::getDeltaTime();
		if (rot.y >= 360) {
			rot.y -= 360;
		}
		rot.z += speed.z * Time::getDeltaTime();
		if (rot.z >= 360) {
			rot.z -= 360;
		}

		(*transform).rotation = rot;
	}
};