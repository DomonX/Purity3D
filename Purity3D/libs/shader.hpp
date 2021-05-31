#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Camera.hpp"
#include "GameState.hpp"

class Shader {
private:
	unsigned int id;
public:
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	void use();
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec3(const std::string& name, const glm::vec3& value) const	{
		glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
	}
	void setVec3(const std::string& name, float x, float y, float z) const {
		glUniform3f(glGetUniformLocation(id, name.c_str()), x, y, z);
	}
	void setMat4(const std::string& name, glm::mat4 mat) const {
		glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
	}
	int getId();
private:
	int compileShader(std::string path, int type);
};