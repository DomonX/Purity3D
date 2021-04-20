#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <string>

#include "ObjLoader.hpp"
#include "Material.hpp"

using namespace std;

class Model {
protected:
	unsigned int VBO;
	unsigned int VAO;
	unsigned int modelSize;
	unsigned int drawMode = GL_TRIANGLES;
	unsigned int modelId;
	unsigned static int modelCounter;
	Material* material;
public:
	Model(string path, Material* material) {

		vector<Vertice> verticiesVec = ObjLoader().load(path);

		modelSize = verticiesVec.size();

		cout << "Loaded Model with " + std::to_string(modelSize) + " Verticies" << endl;

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, modelSize * sizeof(Vertice), &verticiesVec[0], GL_STATIC_DRAW);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);

		this->material = material;

		modelId = Model::modelCounter;
		Model::modelCounter++;
	}

	void setDrawMode(unsigned int mode) {
		drawMode = mode;
	}

	virtual void onUpdate() {
		material->onUpdate();
	}

	virtual void onDraw() { 
		glBindVertexArray(VAO);
		glDrawArrays(drawMode, 0, modelSize);
	}

	Material* getMaterial() {
		return material;
	}
};

unsigned int Model::modelCounter = 0;