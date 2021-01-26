#pragma once

class Component {
public:
	virtual void onGetOtherComponent(Component* component) {}
	virtual void onRemoveOtherComponent(Component component) {}
	virtual void onGetParentComponent(Component* component) {}
	virtual void onRemoveParentComponent(Component* component) {}
	virtual void getGameObject(Component* gameObject) {}
	virtual void onStart() {}
	virtual void onUpdate() {}
	virtual void onDraw() {}
	virtual void onDelete() {}
protected:
	template<class T> void storeIfIsInstance(T** target, Component* component) {
		T* t = dynamic_cast<T*>(component);
		if (!t) {
			return;
		}
		(*target) = t;
	}

	template<class T> void removeIfIsInstance(T** target, Component* component) {
		T* t = dynamic_cast<T*>(component);
		if (t) {
			(*target) = nullptr;
		}
	}

	template<class T> bool isInstance(T** target, Component* component) {
		T* t = dynamic_cast<T*>(component);
		return t;
	}

};