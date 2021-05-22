#pragma once

#include "Component.hpp"
#include "GameObject.hpp"
#include "Transform.hpp"
#include "RidgitBody.hpp"
#include <glm/glm.hpp>

using namespace glm;

class SphereCollider : public Component {
	RidgitBody* body;
public:

	RidgitBody* getBody() {
		return body;
	}

	void onGetOtherComponent(Component* component) {
		storeIfIsInstance(&body, component);
	}

	void getCollision(SphereCollider* col) {
		Transform* tr0 = getBody()->getTransfrom();
		Transform* tr1 = col->getBody()->getTransfrom();
		vec3 scaleTwo = (tr0->scale) + (tr1->scale);
		vec3 distanceVector = tr0->getPosition() - tr1->getPosition();
		vec3 distanceNormal = normalize(distanceVector);
		float radius = length(scaleTwo);
		float dist = length(distanceVector);
		bool hasCollision = dist < scaleTwo.x;
		if (!hasCollision) {
			return;
		}
		float toMove = (scaleTwo.x - dist);
		Transform* tr = getBody()->getTransfrom();
		vec3 moveVector = vec3(-toMove) * distanceNormal;
		tr->setPosition(tr->getRawPosition() - moveVector);
		vec3 force1 = getBody()->getReflectionVector(distanceNormal);
		vec3 force1self = getBody()->getSelfVector();
		vec3 force2 = col->getBody()->getReflectionVector(distanceNormal);
		vec3 force2self = col->getBody()->getSelfVector();
		getBody()->setForce(force2 + force1self);
		col->getBody()->setForce(force1 + force2self);
		return;
	}
};