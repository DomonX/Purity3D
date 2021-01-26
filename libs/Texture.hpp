#pragma once
#include <string>
#include "stb_image.h"

#include "Component.hpp"
#include "shader.hpp"
using namespace std;
class Texture : public Component {
private:
	string path1;
	string path2;
	Shader* shader;
	unsigned int texture1;
	unsigned int texture2;
public:
	Texture(string path1, string path2) {
		this->path1 = path1;
		this->path2 = path2;
		texture1 = createTexture(path1.c_str());
		texture2 = createTexture(path2.c_str());
	}

	void onGetOtherComponent(Component* component) {
		storeIfIsInstance(&shader, component);
		if (isInstance(&shader, component)) {
			shader->use();
			shader->setInt("texture1", 0);
			shader->setInt("texture2", 1);
		}
	}

	void onUpdate() {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
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