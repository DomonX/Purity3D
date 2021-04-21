#pragma once

#include <map>

#include "Model.hpp"
#include "Transform.hpp"

using namespace std;

class DrawElement {
public:
	DrawElement(Model* model) {
		this->model = model;
	}
	Model* model;
	vector<Transform*> transforms;
};

class DrawQueue {
private:
	static DrawQueue* instance;
	map<int, DrawElement*> elements;
public:
	static DrawQueue* get() {
		if (instance) {
			return instance;
		}
		instance = new DrawQueue();
	}

	static void enqueue(Model* model, Transform* transform) {
		DrawQueue* q = DrawQueue::instance;
		DrawElement* e = q->elements[model->getId()];
		if (!e) {
			e = new DrawElement(model);
			q->elements[model->getId()] = e;
		}
		e->transforms.push_back(transform);
	}

	static void draw() {
		for (pair<int, DrawElement*> el : DrawQueue::instance->elements) {
			el.second->model->onDraw();
		}
	}

	static void clear() {
		for (pair<int, DrawElement*> el : DrawQueue::instance->elements) {
			delete el.second;
		}
		DrawQueue::instance->clear();
	}
};

DrawQueue* DrawQueue::instance = nullptr;