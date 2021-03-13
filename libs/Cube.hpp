#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Component.hpp"
#include "Model.hpp"

class Cube : public Model {
protected:
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;

	unsigned int modelSize;
	unsigned int size;

	float* cubeModel;
	int* indicies;

public:
	Cube() {
		glm::vec3 c1 = glm::vec3(-0.5f, -0.5f, -0.5f);
		glm::vec3 c2 = glm::vec3(0.5f, -0.5f, -0.5f);
		glm::vec3 c3 = glm::vec3(0.5f, 0.5f, -0.5f);
		glm::vec3 c4 = glm::vec3(-0.5f, 0.5f, -0.5f);

		glm::vec3 c5 = glm::vec3(-0.5f, -0.5f, 0.5f);
		glm::vec3 c6 = glm::vec3(0.5f, -0.5f, 0.5f);
		glm::vec3 c7 = glm::vec3(0.5f, 0.5f, 0.5f);
		glm::vec3 c8 = glm::vec3(-0.5f, 0.5f, 0.5f);

		float cubeModel[] = {
		//  COORDS			 // TEXTURE			// NORMALS
			c1.x, c1.y, c1.z, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
			c2.x, c2.y, c2.z, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
			c3.x, c3.y, c3.z, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
			c4.x, c4.y, c4.z, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,

			c5.x, c5.y, c5.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			c6.x, c6.y, c6.z, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			c7.x, c7.y, c7.z, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			c8.x, c8.y, c8.z, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

			c8.x, c8.y, c8.z, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
			c4.x, c4.y, c4.z, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
			c1.x, c1.y, c1.z, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
			c5.x, c5.y, c5.z, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

			c7.x, c7.y, c7.z, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
			c3.x, c3.y, c3.z, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			c2.x, c2.y, c2.z, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
			c6.x, c6.y, c6.z, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

			c1.x, c1.y, c1.z, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
			c2.x, c2.y, c2.z, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
			c6.x, c6.y, c6.z, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
			c5.x, c5.y, c5.z, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,

			c4.x, c4.y, c4.z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			c3.x, c3.y, c3.z, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
			c7.x, c7.y, c7.z, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
			c8.x, c8.y, c8.z, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		};

		modelSize = sizeof(cubeModel) / sizeof(float);
		this->cubeModel = cubeModel;

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4,

			8, 9, 10,
			10, 11, 8,

			12, 13, 14,
			14, 15, 12,

			16, 17, 18,
			18, 19, 16,

			20, 21, 22,
			22, 23, 20
		};

		this->indicies = indicies;
		size = sizeof(indices) / sizeof(int);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeModel), cubeModel, GL_STATIC_DRAW);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &EBO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);

	}
	/*! \brief Lifecycle called when frame is beign drawn on screen */
	void onDraw() {
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
	}

private:
};