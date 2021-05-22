#pragma once
#include <string>
#include "stb_image.h"

#include "Component.hpp"
#include "shader.hpp"
#include "GameObject.hpp"
using namespace std;
class Texture : public Component {
private:
	string path1;
	unsigned int texture1;
	GameObject* go = nullptr;
public:
	Texture(string path1) {
		this->path1 = path1;
		texture1 = createTexture(path1.c_str());
	}

	void onUpdate() {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
	}

	void getGameObject(Component* go) {
		this->go = (GameObject*)go;
		this->go->getMaterial()->getShader()->use();
		this->go->getMaterial()->getShader()->setInt("texture1", 0);
	}

	bool isStatic() {
		return true;
	}

private:
	int createTexture(const char* path) {
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		int width, height, nrChannels;
		unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
		int colorMode = nrChannels == 3 ? GL_RGB : GL_RGBA;
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, colorMode, width, height, 0, colorMode, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
		return texture;
	}
};