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
		GameState* gs = GameState::get();
		Camera* cam = (*gs->getCamera());
		glm::mat4 view = (*GameState::get()->getCamera())->GetViewMatrix();
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
		this->use();
	}

private:
	int compileShader(std::string path, int type);
};