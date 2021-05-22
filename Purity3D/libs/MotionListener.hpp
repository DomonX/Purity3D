#pragma once

#include <glm/glm.hpp>
#include "Component.hpp"

using namespace glm;

class MotionListener {
public:
	virtual void objectMoved(Component* object, vec3 dest) {}
};