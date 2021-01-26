#pragma once
#include <string>

#include "Component.hpp"
#include "shader.hpp"
using namespace std;
class Texture : public Component {
private:
	string path1;
	string path2;
	Shader* shader;
public:
	Texture(string path1, string path2) {
		this->path1 = path1;
		this->path2 = path2;
	}

	void onGetOtherComponent(Component* component) {
		storeIfIsInstance(&shader, component);
		if (isInstance(&shader, component)) {
			shader->setInt("texture1", 0);
			shader->setInt("texture2", 1);
		}
	}

	void onUpdate() {

	}
};