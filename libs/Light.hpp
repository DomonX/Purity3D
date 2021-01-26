#pragma once

#include "Cube.hpp"

class Light : Cube {
private:
	unsigned int lightCubeVAO;
public:
	Light() : Cube() {
		glGenVertexArrays(1, &lightCubeVAO);
		glBindVertexArray(lightCubeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
	}
};