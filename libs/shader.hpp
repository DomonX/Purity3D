#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Component.hpp"

class Shader : public Component {
private:
	unsigned int id;
public:
	Shader(const char* vertexPath, const char* fragmentPath);

	void use();

	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

	int getId();

	void onUpdate() {
		this->use();
	}

private:
	int compileShader(std::string path, int type);
};