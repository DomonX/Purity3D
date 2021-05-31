#pragma once
#include <string>

#include "stb_image.h"
#include "shader.hpp"

using namespace std;
class Texture {
private:
	string path1;
	unsigned int texture1;
public:
	Texture(string path1) {
		this->path1 = path1;
		texture1 = createTexture(path1.c_str());
	}

	void onUpdate(Shader* shader) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
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