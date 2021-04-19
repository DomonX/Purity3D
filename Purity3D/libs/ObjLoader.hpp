#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <vector>
#include <iterator>
#include <boost/algorithm/string.hpp>

using namespace std;

struct Vertice {
	glm::vec3 v;
	glm::vec2 t;
	glm::vec3 n;
};

class ObjLoader {
protected:
	vector<glm::vec3> verticies;
	vector<glm::vec2> textures;
	vector<glm::vec3> normals;
	vector<Vertice> result;
public:
	vector<Vertice> load(string path) {
		ifstream obj;
		obj.open(path);
		std::string line;
		while (getline(obj, line)) {
			processLine(line);
		}
		return result;
	}

protected:
	void processLine(string line) {
		if (line.find("v ") != string::npos) {
			addVertice(line.substr(2, line.length()));
		}
		if (line.find("vt ") != string::npos) {
			addTexture(line.substr(3, line.length()));
		}
		if (line.find("vn ") != string::npos) {
			addNormal(line.substr(3, line.length()));
		}
		if (line.find("f ") != string::npos) {
			addIndice(line.substr(2, line.length()));
		}
	}
	void addVertice(string line) {
		vector<string> res;
		boost::split(res, line, [](char c) {return c == ' '; });
		verticies.push_back(glm::vec3(std::stof(res.at(0)), std::stof(res.at(1)), std::stof(res.at(2))));
	}
	void addTexture(string line) {
		vector<string> res;
		boost::split(res, line, [](char c) {return c == ' '; });
		textures.push_back(glm::vec2(std::stof(res.at(0)), std::stof(res.at(1))));
	}
	void addNormal(string line) {
		vector<string> res;
		boost::split(res, line, [](char c) {return c == ' '; });
		normals.push_back(glm::vec3(std::stof(res.at(0)), std::stof(res.at(1)), std::stof(res.at(2))));
	}
	void addIndice(string line) {
		vector<string> res;
		boost::split(res, line, [](char c) {return c == ' '; });
		for (string v : res) {
			addFinalVertice(v);
		}
	}

	void addFinalVertice(string data) {
		vector<string> res;
		boost::split(res, data, [](char c) {return c == '/'; });
		int verticeId = std::stoi(res.at(0));
		int textureId = std::stoi(res.at(1));
		int normalId = std::stoi(res.at(2));
		glm::vec3 vertice = verticies.at(verticeId - 1);
		glm::vec2 texture = textures.at(textureId - 1);
		glm::vec3 normal = normals.at(normalId - 1);
		Vertice v = { vertice , texture , normal };
		result.push_back(v);

	}
};