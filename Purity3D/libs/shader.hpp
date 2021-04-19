#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Component.hpp"
#include "Camera.hpp"
#include "GameState.hpp"

class Shader {
private:
	unsigned int id;
public:
	Shader(const char* vertexPath, const char* fragmentPath);
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
	int getId();
	void onUpdate() {
		GameState* gs = GameState::get();
		Camera* cam = (*gs->getCamera());
		glm::mat4 view = (*GameState::get()->getCamera())->GetViewMatrix();
		use();
		setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		setFloat("material.shininess", 8.0f);
		setVec3("ambient.color", 0.5f, 0.5f, 1.0f);
		setFloat("ambient.brightness", 1.0f);
		setFloat("ambient.min", 0.0f);
		setFloat("ambient.max", 0.1f);
		setVec3("viewPos", cam->Position);

		int viewLoc = glGetUniformLocation(getId(), "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 projection = glm::perspective(
			glm::radians(cam->Zoom), 
			(float)gs->SCR_WIDTH / (float)gs->SCR_HEIGHT, 
			0.1f, 
			100.0f
		);
		int projectionLoc = glGetUniformLocation(getId(), "projection");
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	}
private:
	int compileShader(std::string path, int type);
};